import pytest

import openalea.plantgl.all as pgl


@pytest.mark.skipif(not hasattr(pgl, "DepthSortEngine"), reason="Requires CGAL")
@pytest.mark.parametrize("depths, indexed_height", [
    ((1, 1, 1), 4), ((-1, -1, -1), 4), ((-1, -1, -1), 2), ((-1, 1, -1), 4),
])
def test_depthsort_parallel_preserves_candidate_order(depths, indexed_height):

    def surfaces(threads):
        engine = pgl.DepthSortEngine()
        assert engine.intersectionThreads == 1
        try:
            engine.intersectionThreads = threads
        except (ValueError, RuntimeError) as exc:
            if "without thread support" in str(exc):
                pytest.skip(str(exc))
            raise
        assert engine.intersectionThreads == threads
        engine.setOrthographicCamera(-5, 5, -5, 5, 1, 30)
        engine.lookAt((0, 0, 10), (0, 0, 0), (0, 1, 0))

        def add_triangle(z_values, ident, height=4):
            footprint = [(0, 0), (4, 0), (0, height)]
            points = [pgl.Vector3(x, y, z) for (x, y), z in zip(footprint, z_values)]
            engine.processTriangle(*points, ident)

        # Keep overlapping, independently identified candidates in the index.
        engine.occlusionPolicy = pgl.eOccluded
        for ident in range(24):
            add_triangle((0, 0, 0), ident, indexed_height)

        engine.occlusionPolicy = pgl.eFullOcclusion
        add_triangle(depths, 24)
        return dict(engine.idsurfaces())

    sequential = surfaces(1)
    assert sequential
    assert surfaces(4) == sequential
