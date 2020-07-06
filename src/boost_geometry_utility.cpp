#include <boost_geometry_utility.hpp>

/**
 * get directed angle between the vectors
 */
double boost_geo_util::getDirectedAngle_D(Vector vector1, Vector vector2)
{
    double angle = atan2(vector2.y, vector2.x) - atan2(vector1.y, vector1.x);
    if (angle < 0)
    {
        angle += 2 * PI;
    }
    return (angle * 180) / PI;
}

/**
 * converts a CCW polygon from input into a CW boost Geometry Object
 */
Polygon boost_geo_util::constructBGPolygon(PolygonInput &polygonFromInput)
{
    int n = polygonFromInput.size();
    assert(n >= 3);
    Polygon polygonBG;
    auto startPoint = polygonFromInput[0];
    boost_geo::append(polygonBG.outer(), Point(startPoint.first, startPoint.second));
    std::reverse(polygonFromInput.begin(), polygonFromInput.end()); // make it clockwise
    for (auto point_i : polygonFromInput)
    {
        boost_geo::append(polygonBG.outer(), Point(point_i.first, point_i.second));
    }
    return polygonBG;
}

/**
 * translation to a point
 */
template <class Geometry>
Geometry boost_geo_util::translate(Geometry &geometry, Point point)
{
    Geometry translated;
    trans::translate_transformer<double, 2, 2> translate(point.x, point.y);
    boost_geo::transform(geometry, translated, translate);
    return translated;
}

/**
 * rotation by angle reference to point
 * angle is defined by Degree
 * rotate clockwise
 */
template <class Geometry>
Geometry boost_geo_util::rotate(Geometry &geometry, Point ref, double angle)
{
    Geometry translated;
    boost_geo::multiply_value(ref, -1);
    translated = translate(geometry, ref); // change origin
    Geometry rotated;
    trans::rotate_transformer<boost::geometry::degree, double, 2, 2> rotate(angle); //rotation def
    boost::geometry::transform(translated, rotated, rotate);
    return rotated;
}

/**
 * performs union between two set of polygons
 */
MultiPolygon boost_geo_util::unionPolygons(MultiPolygon &multiPolygonA, MultiPolygon &multiPolygonB)
{
    MultiPolygon multiPolygonUnion;
    boost_geo::union_(multiPolygonA, multiPolygonB, multiPolygonUnion);
    return multiPolygonUnion;
}

/**
 * reflect a set of  polygons about a line
 */
MultiPolygon boost_geo_util::reflectAcrossLine(MultiPolygon &multiPolygon, Point p, Point q)
{
    double x1 = p.x, x2 = q.x;
    double y1 = p.y, y2 = q.y;
    MultiPolygon reflected = multiPolygon;
    for (Polygon &polygon : multiPolygon)
    {
        for (auto it = boost::begin(boost_geo::exterior_ring(polygon));
             it != boost::end(boost_geo::exterior_ring(polygon));
             ++it)
        {
            Point tmpPoint = *it;
            if ((x2 - x1) < EPS)
            {
                *it = Point(2.0 * x1 - tmpPoint.x, tmpPoint.y);
            }
            else if ((y2 - y1) < EPS)
            {
                *it = Point(tmpPoint.x, 2.0 * y1 - tmpPoint.y);
            }
            else
            {
                double m = (y2 - y1) / (x2 - x1);
                double b = y1 - m * x1;

                *it = Point(((1 - m * m) * (tmpPoint.x) + 2 * m * (tmpPoint.y) - 2 * m * b) / (m * m + 1),
                            ((m * m - 1) * (tmpPoint.y) + 2 * m * (tmpPoint.x) + 2 * b) / (m * m + 1));
            }
        }
    }
    return reflected;
}

/**
 * checks for polygon polygon intersection
 */
bool boost_geo_util::isPolygonIntersectPolygon(Polygon &polygonA, Polygon &polygonB)
{
    std::deque<Polygon> output;
    boost_geo::intersection(polygonA, polygonB, output);
    return output.size() == 0;
}

/**
 * generate svg file of polgon 
 * store image in diagrams folder
 */
void boost_geo_util::visualize(MultiPolygon &multipolygon)
{
    std::ostringstream name;
    name << "frame" << std::setw(4) << std::setfill('0') << frameno++ << ".svg";
    std::ofstream ofs("../diagrams" + name.str());
    boost_geo::svg_mapper<Point> mapper(ofs, 400, 400);
    mapper.add(multipolygon);
    mapper.map(multipolygon, "fill-opacity:0.5;fill:rgb(204,153,0);stroke:rgb(204,153,0);stroke-width:1", 5);
}