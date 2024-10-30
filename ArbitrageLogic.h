#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/bellman_ford_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>
#include <iostream>
#include <map>
#include <cmath>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
    boost::property<boost::vertex_name_t, std::string,
    boost::property<boost::vertex_distance_t, double>>,
    boost::property<boost::edge_weight_t, double>> Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

void parse_and_add_to_graph(const std::string& json_str, Graph& g, std::map<std::string, Vertex>& vertex_map);
std::string mock_api_data();
void perform_arbitrage(const char* currency_pair);