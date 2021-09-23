#ifndef FHAMONIC_GEOJSON_DETAIL_HPP
#define FHAMONIC_GEOJSON_DETAIL_HPP

#include <filesystem>
#include <iostream>

#include <boost/geometry.hpp>

#include <simdjson.h>

namespace fhamonic {
namespace geojson {
namespace detail {

template <typename Point, typename T>
Point parse_point(T && t) {
    Point p;
    auto begin = t.begin();
    auto end = t.end();
    if(begin == end) throw std::runtime_error("point with no coordinates");
    p.x((*begin).get_double());
    if(++begin == end) throw std::runtime_error("point with only 1 coordinate");
    p.y((*begin).get_double());
    if(++begin != end)
        throw std::runtime_error("point with more than 2 coordinates");
    return p;
}

template <typename Ring, typename T>
Ring parse_ring(T && ring) {
    using Point = typename boost::geometry::point_type<Ring>::type;
    Ring r;
    for(auto point : ring)
        r.emplace_back(parse_point<Point>(point.get_array()));
    return r;
}

template <typename Polygon, typename T>
Polygon parse_polygon(T && polygon) {
    using Ring = typename boost::geometry::ring_type<Polygon>::type;
    Polygon p;
    auto begin = polygon.begin();
    auto end = polygon.end();
    if(begin == end) throw std::runtime_error("region with empty polygon");
    p.outer() = parse_ring<Ring>((*begin).get_array());
    for(++begin; begin != end; ++begin)
        p.inners().emplace_back(parse_ring<Ring>((*begin).get_array()));
    return p;
}

template <typename Multipolygon, typename T>
Multipolygon parse_multipolygon(T && multipolygon) {
    using Polygon = typename Multipolygon::value_type;
    Multipolygon mp;
    for(auto polygon : multipolygon)
        mp.emplace_back(parse_polygon<Polygon>(polygon.get_array()));
    return mp;
}

template <typename T>
std::vector<std::pair<std::string, std::string>> parse_tags(T && tags) {
    std::vector<std::pair<std::string, std::string>> prop;
    for(auto p : tags)
        prop.emplace_back(p.unescaped_key().value(),
                          p.value().get_string().value());
    return prop;
}

}  // namespace detail
}  // namespace geojson
}  // namespace fhamonic

#endif  // FHAMONIC_GEOJSON_DETAIL_HPP