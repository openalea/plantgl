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

/*
** cdc_ply.h for plantgl in
**
** Made by julien1.benoit@epitech.eu
** Login   <julien1.benoit@epitech.eu>
**
** Started on  jeu. juil. 13:23 2018 Julien Benoit
** Last update jeu. juil. 13:23 2018 Julien Benoit
*/

#ifndef __cdc_ply_h__
#define __cdc_ply_h__

#include "codec_config.h"
#include <plantgl/tool/util_string.h>
#include <plantgl/scenegraph/scene/factory.h>
#include <iostream>
#include <map>
#include <boost/variant.hpp>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

  class CODEC_API PlyCodec : public SceneCodec {
    typedef boost::variant<float, int, uint_t, char, uchar_t, short, u_short, double> propertyType;

    struct SizeVisitor : public boost::static_visitor<> {
      SizeVisitor()
              : size(0) {
      }

      template<typename T>
      void operator()(T &x) const {
        this->size = sizeof(x);
      }

      std::size_t get_size() const {
        return size;
      }

    private:
      mutable std::size_t size;
    };

    struct ReadVisitor : public boost::static_visitor<> {
      ReadVisitor(const std::string &str)
              : str(str) {
      }

      template<typename T>
      void operator()(T &x) const {
        this->value = toNumber<T>(this->str);
      }

      propertyType get_value() const {
        return value;
      }

    private:
      mutable propertyType value;
      const std::string str;
    };

    struct ReinterpretVisitor : public boost::static_visitor<> {
      ReinterpretVisitor(const char *value)
              : value(value) {
      }

      template<typename T>
      void operator()(T &) const {
        res = *(T *) (this->value);
      }

      propertyType get_res() const {
        return res;
      }

    private:
      mutable propertyType res;
      const char *value;
    };

    template<typename U>
    struct GetVisitor : public boost::static_visitor<> {
      GetVisitor() {
      }

      template<typename T>
      void operator()(T &x) const {
        value = (U) x;
      }

      U get_value() const {
        return value;
      }

    private:
      mutable U value;
    };

    struct PropertyElement {
      const bool isList;
      const std::string name;
      const std::string type;
      const std::string sizetype;

      PropertyElement(const std::string &name, const std::string &type)
              : isList(false), name(name), type(type) {
      }

      PropertyElement(const std::string &name, const std::string &sizetype, const std::string &type)
              : isList(true), name(name), type(type), sizetype(sizetype) {
      }

      PropertyElement(const PropertyElement &propertyElement)
              : isList(propertyElement.isList), name(propertyElement.name), type(propertyElement.type), sizetype(propertyElement.sizetype) {
      }

      PropertyElement &operator=(const PropertyElement &propertyElement) {
        return *this;
      }
    };

    struct SpecElement {
      std::size_t number;
      std::vector<PropertyElement> properties;

      SpecElement(const std::size_t &number = 0)
              : number(number) {
      }

      SpecElement(const SpecElement &specElement)
              : number(specElement.number), properties(specElement.properties) {
      }

      SpecElement &operator=(const SpecElement &specElement) {
        this->properties = specElement.properties;
        this->number = specElement.number;

        return *this;
      }
    };

  public:
    PlyCodec();

    virtual SceneFormatList formats() const;

    virtual ScenePtr read(const std::string &fname);

    virtual bool write(const std::string &fname, const ScenePtr &scene);

  private:
    std::string nextline(std::ifstream &file);

    propertyType readnextval(std::ifstream &file, const propertyType &type, const bool &reversebytes);

    bool colorPropsSortFunction(const std::string &c1, const std::string &c2);

    std::vector<std::string> fcodingTypes;
    std::map<std::string, propertyType> propertiesTypes;
    std::vector<std::string> knowncolortypes;
    SizeVisitor sizeVisitor;
  };

PGL_END_NAMESPACE

#endif /* !__cdc_ply_h__ */
