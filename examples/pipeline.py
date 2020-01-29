import openalea.plantgl.scenegraph._pglsg as pglsg
import openalea.plantgl.algo._pglalgo as pglalgo
import openalea.plantgl.math._pglmath as pglmath
import sys
import math
import random
import numpy

red = pglsg.Material("red", pglsg.Color3(255, 0, 0))
green = pglsg.Material("green", pglsg.Color3(0, 255, 0))
blue = pglsg.Material("blue", pglsg.Color3(0, 0, 255))
purple = pglsg.Material("purple", pglsg.Color3(50, 0, 50))
orange = pglsg.Material("orange", pglsg.Color3(100, 65, 0))
yellow = pglsg.Material("yellow", pglsg.Color3(255, 255, 0))
maroon = pglsg.Material("maroon", pglsg.Color3(50, 0, 0))
pink = pglsg.Material("pink", pglsg.Color3(100, 75, 80))


def np_inertia_axis(points, verbose=False):
    assert len(points) > 0
    import numpy as np
    import openalea.plantgl.all as pgl
    if type(points) != pgl.Point3Array:
        points = pgl.Point3Array(points)

    if verbose: print 'centering points'
    center = points.getCenter()
    npoints = pgl.Point3Array(points)
    if pgl.norm(center) > 1e-5:
        npoints.translate(- center)

    if verbose: print 'compute covariance'
    # compute 1/N*P.P^T
    # cov = 1./len(points)*np.dot(npoints.T,npoints)
    cov = pgl.pointset_covariance(npoints)
    cov = np.array([cov.getRow(i) for i in xrange(3)])
    if verbose: print cov

    if verbose: print "compute eigen vectors"
    # Find the eigen values and vectors.
    eig_val, eig_vec = np.linalg.eig(cov)

    if verbose:
        for i in xrange(3):
            print eig_val[i], eig_vec[:, i]

    eig_vec = np.array(eig_vec).T

    eig_vec = np.array([eig_vec[i] for i in reversed(pgl.get_sorted_element_order(eig_val))])
    eig_val = np.array([eig_val[i] for i in reversed(pgl.get_sorted_element_order(eig_val))])

    return eig_val, eig_vec, center


def align_points(point):
    """

    Parameters
    ----------
    point : pglsg.Point3Array
    """

    maxZ = point[point.getZMaxIndex()].z
    minZ = point[point.getZMinIndex()].z
    height = maxZ - minZ
    filterpoint = point.filterCoordinates(2, minZ, minZ + height * 0.2)

    eig_val, eig_vec, center = np_inertia_axis(filterpoint)
    ppos = pglsg.Point3Array(point)
    ppos.translate(-center)
    ppos.transform(pglmath.Matrix3(eig_vec[1], eig_vec[0], eig_vec[2]).inverse())
    return ppos


def get_baricenters(point):
    kclosest_wire = pglsg.IndexArray(0)

    pmin, pmax = pglalgo.find_min_max(point, 3)
    wire_path = pglalgo.get_shortest_path(point, kclosest_wire, pmin, pmax)  # type: pglsg.Index
    wirepoints, p = point.split_subset(wire_path)  # type: (pglsg.Point3Array, pglsg.Point3Array)

    wire_orient = pglalgo.pointset_orientation(point, wire_path)
    wire_center = wirepoints.getCenter()
    pmin, pmax = pglalgo.find_min_max(point, 3, wire_center, wire_orient)
    wire_path = pglalgo.get_shortest_path(point, kclosest_wire, pmin, pmax)  # type: pglsg.Index

    return pglalgo.add_baricenter_points_of_path(point, kclosest_wire, wire_path, 0.04)


def select_wire(point):
    """

    Parameters
    ----------
    point : pglsg.Point3Array
    """

    newpoint, baricenters = get_baricenters(point)
    kclosest = pglalgo.k_closest_points_from_ann(newpoint, 20)

    radii = pglalgo.get_radii_of_path(newpoint, kclosest, baricenters, 0.05)
    radius = numpy.average(radii)

    wire = pglalgo.select_wire_from_path(newpoint, baricenters, radius, radii)
    return newpoint.split_subset(wire)


def select_pole(point):
    pole, score = pglalgo.select_pole_points(point, 0.15, 10000)
    return point.split_subset(pole)


def select_bigest_connex_component(point):
    kclosest = pglalgo.k_closest_points_from_ann(point, 10, True)
    connexs = pglalgo.get_all_connex_components(point, kclosest)

    maxSize = -1
    maxSizeIndex = 0
    for i in range(len(connexs)):
        if maxSize == -1 or maxSize < len(connexs[i]):
            maxSize = len(connexs[i])
            maxSizeIndex = i

    return point.split_subset(connexs[maxSizeIndex])


def select_isolate_point(point, minDensity):
    kclosest = pglalgo.k_closest_points_from_ann(point, 20)
    # rneighborhoods = pglalgo.r_neighborhoods_mt(point, kclosest, 0.01, True)
    isolate_point = pglalgo.select_k_isolate_points(point, kclosest, 20, minDensity)
    return point.split_subset(isolate_point)


def segment_trees(point):
    kclosest = pglalgo.k_closest_points_from_ann(point, 10, True)
    connexsIndex = pglalgo.get_all_connex_components(point, kclosest)

    connexs = []  # type: List[pglsg.Point3Array]
    for c in connexsIndex:
        if len(c) < 10000:
            continue
        connexs.append(point.split_subset(c)[0])

    mats = []  # type: List[pglsg.Material]
    while len(mats) != len(connexs):
        r = random.randrange(0, 256)
        g = random.randrange(0, 256)
        b = random.randrange(0, 256)
        color = pglsg.Color3(r, g, b)
        inmats = False
        for m in mats:
            if m.ambient == color:
                inmats = True
                break
        if not inmats:
            mats.append(pglsg.Material("mat" + str(len(mats)), color))

    return connexs, mats


def select_poles(point):
    kclosest = pglalgo.k_closest_points_from_ann(point, 5, True)
    connexsIndex = pglalgo.get_all_connex_components(point, kclosest)  # type: pglsg.IndexArray
    poleIndexsAndScore = []
    polesPoints = pglsg.Point3Array(0)
    otherPoints = pglsg.Point3Array(0)
    for connex in connexsIndex:
        connexPoint = point.split_subset(connex)[0]  # type: pglsg.Point3Array
        if len(connex) < 10000:
            otherPoints.append(connexPoint)
            continue
        pole, score = pglalgo.select_pole_points(connexPoint, 0.175, 10000, 0.6)
        if score < 0.9:
            otherPoints.append(connexPoint)
            continue
        score = float(int(score * 10000)) / 10000
        poleIndexsAndScore.append([pole, score])
        polesPoints.append(connexPoint)

    return polesPoints, otherPoints


if __name__ == '__main__':
    if len(sys.argv) < 2:
        sys.exit(1)

    s = pglsg.Scene(sys.argv[1])  # type: pglsg.Scene
    p = s[0].geometry.geometry.pointList  # type: pglsg.Point3Array

    isolatepoints, pointClear = select_isolate_point(p, 5000)
    p1, not_selected_point = select_bigest_connex_component(pointClear)

    p2 = align_points(p1)

    notground = pglalgo.select_soil(p2, pglsg.IndexArray(0), 10)  # type: pglsg.Index
    p3, groundpoints = p2.split_subset(notground)  # type: (pglsg.Point3Array, pglsg.Point3Array)

    isolatepoints, pointClear = select_isolate_point(p3, 20000)

    wirepoints1, p4 = select_wire(pointClear)  # type: (pglsg.Point3Array, pglsg.Point3Array)
    isolatepoints, pointClear = select_isolate_point(p4, 20000)
    # p5, not_selected_point = select_bigest_connex_component(pointClear)

    wirepoints2, p5 = select_wire(pointClear)  # type: (pglsg.Point3Array, pglsg.Point3Array)
    isolatepoints, pointClear = select_isolate_point(p5, 20000)

    polespoints, p6 = select_poles(pointClear)

    isolatepoints, pointClear = select_isolate_point(p6, 30000)

    connexs, materials = segment_trees(pointClear)

    shapesConnexs = []  # type: List[pglsg.Shape]
    for i in range(len(connexs)):
        shapesConnexs.append(pglsg.Shape(pglsg.PointSet(connexs[i]), materials[i]))

    shapeGround = pglsg.Shape(pglsg.PointSet(groundpoints), red)
    shapeWirepoints1 = pglsg.Shape(pglsg.PointSet(wirepoints1), purple)
    shapeWirepoints2 = pglsg.Shape(pglsg.PointSet(wirepoints2), orange)
    shapePoles = pglsg.Shape(pglsg.PointSet(polespoints), maroon)

    shapes = [shapesConnexs[i] for i in range(len(shapesConnexs))]
    shapes.append(shapeWirepoints1)
    shapes.append(shapeWirepoints2)
    shapes.append(shapePoles)
    scene = pglsg.Scene(shapes)
