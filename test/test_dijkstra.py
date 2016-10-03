
topology = [[1,2,3],     # 0
            [0,2],       # 1
            [0,1,3,4,5], # 2
            [0,2,6],     # 3
            [2,5,8],     # 4
            [2,4,7],     # 5
            [3,7],       # 6
            [5,6,9],     # 7
            [4,9],       # 8
            [7,8]]       # 9

distances =  {(0,1) : 1, (0,2) : 1.5, (0,3) : 2.25, (1,2): 1, (2,3) : 1, (2,4) : 1, (2,5): 1.5, (3,6) : 1.5, (4,5) : 1, (4,8) : 1.5, (5,7) : 1.5, (6,7) : 1, (8,9) : 1.5, (7,9) : 1.5}

def distance(a,b): return distances[(min(a,b),max(a,b))] 

resparents = [0,0,0,0,2,2,3,5,4,8]
resdists   = [0.,1.,1.5,2.25,2.5,3.0,3.75,4.5,4.0,5.5]

def test_dijkstra_shortest_paths():
    from openalea.plantgl.all import     dijkstra_shortest_paths   
    parents, mindists = dijkstra_shortest_paths(topology, 0, distance)
    print 'dijkstra_shortest_paths'
    for i in xrange(len(parents)):
        print i, parents[i], mindists[i]
    assert list(parents)  == resparents
    assert list(mindists) == resdists

def test_dijkstra_shortest_paths_in_a_range():
    from openalea.plantgl.all import     dijkstra_shortest_paths_in_a_range   
    results = dijkstra_shortest_paths_in_a_range(topology, 0, distance)
    results.sort(key=lambda x:x[0])
    print 'dijkstra_shortest_paths_in_a_range'
    print results
    for nid, parent, dist in results:
        print nid, parent, dist
    assert [p for i,p,d in results] == resparents
    assert [d for i,p,d in results] == resdists

def test_dijkstra_shortest_paths_in_a_range2():
    from openalea.plantgl.all import     dijkstra_shortest_paths_in_a_range   
    maxdist = 4.8
    results = dijkstra_shortest_paths_in_a_range(topology, 0, distance, maxdist)
    results.sort(key=lambda x:x[0])
    print 'dijkstra_shortest_paths_in_a_range'
    print results
    for nid, parent, dist in results:
        print nid, parent, dist
    idx = max([i for i, val in enumerate(resdists) if val <= maxdist])
    assert [p for i,p,d in results] == resparents[:idx+1]
    assert [d for i,p,d in results] == resdists[:idx+1]

def test_dijkstra_shortest_paths_in_a_range3():
    from openalea.plantgl.all import     dijkstra_shortest_paths_in_a_range   
    maxnbelem = 5
    results = dijkstra_shortest_paths_in_a_range(topology, 0, distance, 100, maxnbelements = maxnbelem)
    results.sort(key=lambda x:x[0])
    print 'dijkstra_shortest_paths_in_a_range'
    print results
    for nid, parent, dist in results:
        print nid, parent, dist
    assert [p for i,p,d in results] == resparents[:maxnbelem]
    assert [d for i,p,d in results] == resdists[:maxnbelem]

from openalea.plantgl.all import *


# def test_dijkstra_shortest_paths_big_data():
#     s = Scene('/Users/fboudon/Develop/oagit/plantscan3d/data/pointset/arabido-yassin-200k.bgeom')
#     print
#     pointList = s[0].geometry.pointList
#     kclosests = k_closest_points_from_ann(pointList, 7, True)
#     res = r_neighborhoods(pointList, kclosests, 0.5)
#     #res = sorted(list(res))
#     #print len(res)
#     #print res
#     ref = pgl_load_data("rnbgs-fibo.bpgl")
#     print 'compare'
#     i = 0
#     for rn, rf in zip(res,ref):
#         if list(rn) != list(rf):
#             print i, list(rn), list(rf)
#     return res



if __name__ == '__main__':
    test_dijkstra_shortest_paths()
    test_dijkstra_shortest_paths_in_a_range()
    test_dijkstra_shortest_paths_in_a_range2()    
    test_dijkstra_shortest_paths_in_a_range3()
    test_dijkstra_shortest_paths_big_data()