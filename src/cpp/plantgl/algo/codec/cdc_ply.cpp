/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright CIRAD/INRIA/INRA
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *  ----------------------------------------------------------------------------
 *
 *   This software is governed by the CeCILL-C license under French law and
 *   abiding by the rules of distribution of free software.  You can  use,
 *   modify and/ or redistribute the software under the terms of the CeCILL-C
 *   license as circulated by CEA, CNRS and INRIA at the following URL
 *   "http://www.cecill.info".
 *
 *   As a counterpart to the access to the source code and  rights to copy,
 *   modify and redistribute granted by the license, users are provided only
 *   with a limited warranty  and the software's author,  the holder of the
 *   economic rights,  and the successive licensors  have only  limited
 *   liability.
 *
 *   In this respect, the user's attention is drawn to the risks associated
 *   with loading,  using,  modifying and/or developing or reproducing the
 *   software by the user in light of its specific status of free software,
 *   that may mean  that it is complicated to manipulate,  and  that  also
 *   therefore means  that it is reserved for developers  and  experienced
 *   professionals having in-depth computer knowledge. Users are therefore
 *   encouraged to load and test the software's suitability as regards their
 *   requirements in conditions enabling the security of their systems and/or
 *   data to be ensured and,  more generally, to use and operate it in the
 *   same conditions as regards security.
 *
 *   The fact that you are presently reading this means that you have had
 *   knowledge of the CeCILL-C license and that you accept its terms.
 *
 *  ----------------------------------------------------------------------------
 */

#include "cdc_ply.h"
#include <plantgl/tool/bfstream.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/colorarray.h>
#include <plantgl/scenegraph/geometry/pointset.h>
#include <plantgl/scenegraph/geometry/faceset.h>
#include <plantgl/tool/util_progress.h>
#include "plyprinter.h"
#include <stdexcept>
#include <boost/bind.hpp>
#include <boost/function.hpp>

PGL_USING_NAMESPACE

PlyCodec::PlyCodec() :
	SceneCodec("PLY", ReadWrite),
	m_reverseBytes(false)
{
	m_fcodingTypes.push_back("binary_little_endian");
	m_fcodingTypes.push_back("binary_big_endian");
	m_fcodingTypes.push_back("ascii");

	m_propertiesTypes["float"] = float();
	m_propertiesTypes["float32"] = float();
	m_propertiesTypes["int"] = int();
	m_propertiesTypes["int8"] = int8_t();
	m_propertiesTypes["int16"] = int16_t();
	m_propertiesTypes["int32"] = int32_t();
	m_propertiesTypes["uint"] = uint_t();
	m_propertiesTypes["uint8"] = uint8_t();
	m_propertiesTypes["uint16"] = uint16_t();
	m_propertiesTypes["uint32"] = uint32_t();
	m_propertiesTypes["uchar"] = uchar_t();
	m_propertiesTypes["char"] = char();
	m_propertiesTypes["short"] = short();
	m_propertiesTypes["ushort"] = uint16_t();
	m_propertiesTypes["double"] = double();

	m_knownColorTypes.push_back("red");
	m_knownColorTypes.push_back("green");
	m_knownColorTypes.push_back("blue");
	m_knownColorTypes.push_back("diffuse_red");
	m_knownColorTypes.push_back("diffuse_green");
	m_knownColorTypes.push_back("diffuse_blue");
	m_knownColorTypes.push_back("alpha");
}

SceneFormatList PlyCodec::formats() const
{
	SceneFormat format;
	
	format.name = "Ply Codec";
	format.suffixes.push_back("ply");
	format.comment = "The ply format.";
	
	SceneFormatList formats;
	formats.push_back(format);
	
	return formats;
}

ScenePtr PlyCodec::read(std::string const &fname)
{
	try {
		return readScene(fname);
	}
	catch (std::exception const &) {
		return ScenePtr();
	}
}

ScenePtr PlyCodec::readScene(std::string const &fname)
{
	std::ifstream file = openFile(fname);

	FormatInfos const format = parseFormatInfos(file);

	m_reverseBytes = isBytesReverseNeeded(format.coding);

	std::map<std::string, SpecElement> specs = parseHeader(file, format);

	m_colorProps = parseColorProps(specs);

	// Data size
	std::size_t const vertexSize = specs.find("vertex") != specs.end() ? specs["vertex"].number : 0;
	std::size_t const colorSize = !m_colorProps.empty() ? vertexSize : 0;
	std::size_t const faceSize = specs.find("face") != specs.end() ? specs["face"].number : 0;

	// Data
	Point3ArrayPtr const points = new Point3Array(vertexSize);
	Color4ArrayPtr const colors = new Color4Array(colorSize);
	IndexArrayPtr const faces = new IndexArray(faceSize);

	for (std::map<std::string, SpecElement>::const_iterator it = specs.begin(); it != specs.end(); ++it) {
		// Parsing progression
		ProgressStatus status(it->second.number, "Loading PLY file.", 0.25f);

		for (std::size_t i = 0; i < it->second.number; ++i, ++status) {
			// The element being processed
			pgl_hash_map_string<std::vector<propertyType> > element;

			if (format.coding == "ascii") {
				// ASCII
				parseAsciiValue(file, it, element);
			}
			else {
				// Binary
				parseBinaryValue(file, it, element);
			}

			if (it->first == "vertex") {
				// Vertex
				parseVertex(i, element, points, colors);
			}
			else if (it->first == "face") {
				// Face
				parseFace(i, element, faces);
			}
		}
	}

	return createScene(points, colors, faces);
}

void PlyCodec::parseAsciiValue(std::ifstream& file, std::map<std::string, SpecElement>::const_iterator const &it, pgl_hash_map_string<std::vector<propertyType> > &element)
{
	std::vector<std::string> const lineValues = split(readNextLine(file));
	std::size_t itv = 0;
	
	for (std::vector<PropertyElement>::const_iterator propIt = it->second.properties.begin(); propIt != it->second.properties.end(); ++propIt) {
		// We check if the property type exists
		pgl_hash_map_string<propertyType>::const_iterator const typeIt = m_propertiesTypes.find(propIt->type);

		if (typeIt == m_propertiesTypes.end()) {
			// Unknown property
			continue;
		}

		// Here we insert the new vector
		std::pair<pgl_hash_map_string<std::vector<propertyType> >::iterator, bool> const vect = element.insert(std::pair<std::string, std::vector<propertyType> >(propIt->name, std::vector<propertyType>()));

		// A reference to the vector containing the values
		std::vector<propertyType> &values = vect.first->second;

		propertyType type = typeIt->second;

		if (propIt->isList) {
			std::size_t const propCount = toNumber<std::size_t>(lineValues[itv++]);

			for (std::size_t i = 0; i < propCount; ++i) {
				ReadVisitor visitor(lineValues[itv++]); 
				boost::apply_visitor(visitor)(type);
				
				values.push_back(visitor.get_value());
			}
		}
		else {
			ReadVisitor visitor(lineValues[itv++]);
			boost::apply_visitor(visitor)(type);
			
			values.push_back(visitor.get_value());
		}
	}
}

void PlyCodec::parseBinaryValue(std::ifstream& file, std::map<std::string, SpecElement>::const_iterator const &it, pgl_hash_map_string<std::vector<propertyType> > &element)
{
	for (std::vector<PropertyElement>::const_iterator propIt = it->second.properties.begin(); propIt != it->second.properties.end(); ++propIt) {
		// We check if the property type exists
		pgl_hash_map_string<propertyType>::const_iterator const typeIt = m_propertiesTypes.find(propIt->type);

		if (typeIt == m_propertiesTypes.end()) {
			// Unknown property
			continue;
		}

		if (propIt->isList) {
			// Here we insert the new vector
			std::pair<pgl_hash_map_string<std::vector<propertyType> >::iterator, bool> const vect = element.insert(std::pair<std::string, std::vector<propertyType> >(propIt->name, std::vector<propertyType>()));

			// A reference to the vector containing the values
			std::vector<propertyType>& values = vect.first->second;

			propertyType const sizeType = m_propertiesTypes[propIt->sizetype];
			GetVisitor<size_t> visitor;
			
			propertyType const tmp = readNextValue(file, sizeType);
			tmp.apply_visitor(visitor);

			std::size_t const propCount = visitor.get_value();

			for (std::size_t i = 0; i < propCount; ++i) {
				values.push_back(readNextValue(file, typeIt->second));
			}
		}
		else {
			element.insert(std::pair<std::string, std::vector<propertyType> >(propIt->name, std::vector<propertyType>(1, readNextValue(file, typeIt->second))));
		}
	}
}

void PlyCodec::parseVertex(std::size_t i, pgl_hash_map_string<std::vector<propertyType> > &element, Point3ArrayPtr const points, Color4ArrayPtr const colors)
{
	GetVisitor<float> visitor;

	element["x"][0].apply_visitor(visitor);
	float const x = visitor.get_value();
	
	element["y"][0].apply_visitor(visitor);
	float const y = visitor.get_value();
	
	element["z"][0].apply_visitor(visitor);
	float const z = visitor.get_value();
	
	points->setAt(i, Vector3(x, y, z));
	
	if (m_colorProps.empty()) {
		return;
	}

	uchar_t rgba[4];
	std::size_t index = 0;

	for (std::vector<std::string>::const_iterator cni = m_colorProps.begin(); cni != m_colorProps.end(); ++cni, ++index) {
		GetVisitor<uchar_t> v;
		element[*cni][0].apply_visitor(v);
		rgba[index] = v.get_value();
	}

	if (m_colorProps.size() >= 4 && m_colorProps[3] == "alpha") {
		rgba[3] = 255 - rgba[3];
	}
	else {
		rgba[3] = 0;
	}

	colors->setAt(i, Color4(rgba));
}

void PlyCodec::parseFace(std::size_t i, pgl_hash_map_string<std::vector<propertyType> > const &element, IndexArrayPtr const faces)
{
	pgl_hash_map_string<std::vector<propertyType> >::const_iterator it = element.find("vertex_indices");
	
	GetVisitor<uint_t> visitor;
	Index index;
	
	for (std::vector<propertyType>::const_iterator v = it->second.begin(); v != it->second.end(); ++v) {
		v->apply_visitor(visitor);
		index.push_back(visitor.get_value());
	}
	
	faces->setAt(i, index);
}

std::ifstream PlyCodec::openFile(std::string const& path) const
{
	std::ifstream file(path.c_str(), std::ios::binary);

	if (!file) {
		throw std::runtime_error("Could not open file: " + path);
	}

	std::string const format = readNextLine(file);

	if (strip(format) != "ply") {
		// The file header must start by 'ply' to be considered valid
		throw std::runtime_error("Invalid format");
	}

	return file;
}

PlyCodec::FormatInfos PlyCodec::parseFormatInfos(std::ifstream &file) const
{
	std::vector<std::string> const format = split(readNextLine(file));

	if (format.size() < 3) {
		// Informations are missing
		throw std::runtime_error("Truncated header");
	}

	FormatInfos infos;

	infos.keyword = format[0];
	infos.coding = format[1];
	
	if (std::find(m_fcodingTypes.begin(), m_fcodingTypes.end(), infos.coding) == m_fcodingTypes.end()) {
		// Unknown coding type
		throw std::runtime_error("Invalid format");
	}

	infos.version = toNumber<float>(format[2]);

	return infos;
}

std::map<std::string, PlyCodec::SpecElement> PlyCodec::parseHeader(std::ifstream &file, FormatInfos const &format) const
{
	std::map<std::string, SpecElement> specs;

	std::string lastSpecName;

	for (std::string line = readNextLine(file); line != "end_header" && file.good(); line = readNextLine(file)) {
		if (line.find("element") == 0) {
			lastSpecName = parseHeaderElement(specs, line);
		}
		else if (line.find("property") == 0) {
			parseHeaderProperty(specs, line, lastSpecName);
		}
	}

	return specs;
}

std::string PlyCodec::parseHeaderElement(std::map<std::string, SpecElement> &specs, std::string const& line) const
{
	std::vector<std::string> const elements = split(line);

	std::string const &name = elements[1];
	std::string const &number = elements[2];

	specs[name] = SpecElement(toNumber<std::size_t>(number));
	return name;
}

void PlyCodec::parseHeaderProperty(std::map<std::string, SpecElement> &specs, std::string const& line, std::string const& lastSpecName) const
{
	std::vector<std::string> const properties = split(line);

	if (properties.size() == 3) {
		std::string const &type = properties[1];
		std::string const &name = properties[2];
		
		if (m_propertiesTypes.find(type) == m_propertiesTypes.end()) {
			// Invalid property type
			throw std::runtime_error("Invalid header");
		}
		
		specs[lastSpecName].properties.push_back(PropertyElement(name, type));
	}
	else if (properties.size() == 5) {
		std::string const &sizeType = properties[2];
		std::string const &type = properties[3];
		std::string const &name = properties[4];

		if (m_propertiesTypes.find(sizeType) == m_propertiesTypes.end() || m_propertiesTypes.find(type) == m_propertiesTypes.end()) {
			// Invalid property type
			throw std::runtime_error("Invalid header");
		}

		specs[lastSpecName].properties.push_back(PropertyElement(name, sizeType, type));
	}
}

std::vector<std::string> PlyCodec::parseColorProps(std::map<std::string, SpecElement> const& specs) const
{
	std::vector<std::string> colorProps;

	std::map<std::string, SpecElement>::const_iterator const it = specs.find("vertex");

	if (it != specs.end()) {
		SpecElement const &vertex = it->second;

		for (std::size_t i = 2; i < vertex.properties.size(); ++i) {
			if (std::find(m_knownColorTypes.begin(), m_knownColorTypes.end(), vertex.properties[i].name) != m_knownColorTypes.end()) {
				colorProps.push_back(vertex.properties[i].name);
			}
		}
	}
	
	if (!colorProps.empty()) {
		std::sort(colorProps.begin(), colorProps.end(), boost::bind(&PlyCodec::colorPropsSortFunction, this, _1, _2));

		if (colorProps.size() != 3 && colorProps.size() != 4) {
			// Invalid layer count
			throw std::runtime_error("Invalid header");
		}
	}

	return colorProps;
}

bool PlyCodec::isBytesReverseNeeded(std::string const &fcoding) const
{
	if (fcoding == "ascii") {
		// Not byte order reverse in ASCII mode
		return false;
	}
	
	std::vector<std::string> const data = split(fcoding, "_");
		
	if (data.size() < 2) {
		// Byte order not specified
		return false;
	}

	std::string const &byteorder = data[1];

	return (byteorder == "little" && __BYTE_ORDER == __BIG_ENDIAN) || (byteorder == "big" && __BYTE_ORDER == __LITTLE_ENDIAN);
}


ScenePtr PlyCodec::createScene(Point3ArrayPtr points, Color4ArrayPtr colors, IndexArrayPtr faces) const
{
	ScenePtr const scene = new Scene;
	ShapePtr shape = NULL;

	if (faces->empty()) {
		shape = new Shape(GeometryPtr(new PointSet(points, colors)));
	}
	else {
		shape = new Shape(GeometryPtr(new FaceSet(points, faces)));
	}
	
	scene->add(shape);
	return scene;
}

bool PlyCodec::write(std::string const &fname, ScenePtr const &scene)
{
	return PlyPrinter::print(scene, fname, NULL, PlyPrinter::ply_binary_little_endian);
}

std::string PlyCodec::readNextLine(std::ifstream &file) const
{
	std::string line;
	
	do {
		std::getline(file, line);
	}
	// We ignore comments
	while (line.find("comment") == 0);
	
	return strip(line);
}

PlyCodec::propertyType PlyCodec::readNextValue(std::ifstream &file, propertyType const &type)
{
	boost::apply_visitor(sizeVisitor)(type);
	
	std::size_t const size = sizeVisitor.get_size();
	// Total size: source + destination
	std::size_t const totalSize = size * 2;

	if (totalSize > m_buffer.size()) {
		// If required, we resize the buffer
		m_buffer.resize(totalSize);
	}

	char *const buffer = &m_buffer[0];

	file.read(buffer, size);

	if (m_reverseBytes) {
		// Switch endianness
		flipBytes(buffer, buffer + size, size);
	}
	else {
		std::memcpy(buffer + size, buffer, size);
	}

	ReinterpretVisitor const visitor(buffer + size);
	boost::apply_visitor(visitor)(type);
	
	return visitor.get_res();
}

bool PlyCodec::colorPropsSortFunction(std::string const &c1, std::string const &c2) const
{
	std::size_t const indexc1 = std::distance(m_knownColorTypes.begin(), std::find(m_knownColorTypes.begin(), m_knownColorTypes.end(), c1));
	std::size_t const indexc2 = std::distance(m_knownColorTypes.begin(), std::find(m_knownColorTypes.begin(), m_knownColorTypes.end(), c2));

	return indexc1 < indexc2;
}
