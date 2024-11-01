//=======================================================================
// Copyright 2024 Christian B. Ochoa
//=======================================================================

#include "ArbitrageLogic.h"

double initial_amount = 1000.0;

// Mock function to return API data in JSON format
std::string mock_api_data() {
    return R"([
        {"from": "USD", "to": "EUR", "rate": 1.1},
        {"from": "EUR", "to": "USD", "rate": 0.9},
        {"from": "EUR", "to": "JPY", "rate": 0.9},
        {"from": "JPY", "to": "EUR", "rate": 1.1},
        {"from": "JPY", "to": "USD", "rate": 110},
        {"from": "USD", "to": "JPY", "rate": 0.009}
    ])";
}

// Function to parse JSON string and add data to the graph
void parse_and_add_to_graph(const std::string& json_str, Graph& g, std::map<std::string, Vertex>& vertex_map) {
    std::stringstream ss(json_str);
    std::string item;

    while (getline(ss, item, '{')) {
        // Parse Data
        if (item.find("from") == std::string::npos) continue;

        auto from_pos = item.find("\"from\": \"") + 8;
        auto from_end = item.find("\"", from_pos);
        std::string from_currency = item.substr(from_pos, from_end - from_pos);

        auto to_pos = item.find("\"to\": \"") + 6;
        auto to_end = item.find("\"", to_pos);
        std::string to_currency = item.substr(to_pos, to_end - to_pos);

        auto rate_pos = item.find("\"rate\": ") + 8;
        auto rate_end = item.find(",", rate_pos);
        double rate = std::stod(item.substr(rate_pos, rate_end - rate_pos));

        // Add Data to Graph
        Vertex from, to;

        // Add Verticies
        if (vertex_map.find(from_currency) == vertex_map.end()) {
            from = boost::add_vertex(from_currency, g);
            vertex_map[from_currency] = from;
        }
        else {
            from = vertex_map[from_currency];
        }

        if (vertex_map.find(to_currency) == vertex_map.end()) {
            to = boost::add_vertex(to_currency, g);
            vertex_map[to_currency] = to;
        }
        else {
            to = vertex_map[to_currency];
        }

        // Add Edges
        double weight = -std::log(rate);
        boost::add_edge(from, to, weight, g);
    }
}

// Main function to find arbitrage opportunity
// From USD to USD Only at this time, will add ability to choose starting and ending 
void perform_arbitrage() {
    std::string json_data = mock_api_data();

    Graph g;
    std::map<std::string, Vertex> vertex_map;
    parse_and_add_to_graph(json_data, g, vertex_map);

    std::vector<double> distances(boost::num_vertices(g), std::numeric_limits<double>::max());
    Vertex start_vertex = *boost::vertices(g).first;
    distances[start_vertex] = 0;

    std::vector<Vertex> predecessors(boost::num_vertices(g), boost::graph_traits<Graph>::null_vertex());

    bool has_negative_cycle = boost::bellman_ford_shortest_paths(g, boost::num_vertices(g),
        boost::weight_map(boost::get(boost::edge_weight, g))
        .distance_map(&distances[0])
        .predecessor_map(&predecessors[0]));

    if (has_negative_cycle) {
        std::cout << "Arbitrage opportunity detected!" << std::endl;
        for (auto v : boost::make_iterator_range(boost::vertices(g))) {
            std::cout << boost::get(boost::vertex_name, g, v) << " distance: " << distances[v] << std::endl;
        }

        for (Vertex v = start_vertex; ; v = predecessors[v]) {
            std::cout << "Visiting: " << boost::get(boost::vertex_name, g, v) << std::endl;
            if (v == start_vertex && distances[v] < 0) break;
        }
        std::cout << "Initial Amount: " << initial_amount << " Final Amount: " << initial_amount * exp(-distances[start_vertex]) << std::endl;
    }
    else {
        std::cout << "No arbitrage opportunities." << std::endl;
    }
}
