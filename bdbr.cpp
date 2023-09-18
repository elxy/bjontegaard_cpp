/*++
    BDcpp -- Simple Bjontegaard Delta metric implementation for C++.

    Example code to demonstrate usage.

    Copyright (c) Tim bruylants. All rights reserved.
--*/

#include "bdcpp.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include <argparse/argparse.hpp>

bdcpp::value_type round(const bdcpp::value_type& v, const int digits)
{
    const double p = std::pow(10., digits);    
    return (bdcpp::value_type)((int64_t)(v * p + (v > 0 ? 0.5 : -0.5)) / p);
}

argparse::ArgumentParser parse_args(int argc, char* argv[]) {
    argparse::ArgumentParser parser;

    parser.add_argument("--ref_rate").nargs(argparse::nargs_pattern::at_least_one).scan<'g', float>();
    parser.add_argument("--ref_quality").nargs(argparse::nargs_pattern::at_least_one).scan<'g', float>();
    parser.add_argument("--test_rate").nargs(argparse::nargs_pattern::at_least_one).scan<'g', float>();
    parser.add_argument("--test_quality").nargs(argparse::nargs_pattern::at_least_one).scan<'g', float>();

    parser.add_argument("--piecewise")
        .default_value(false)
        .implicit_value(true);

    parser.parse_args(argc, argv);

    return parser;
}

int main(int argc, char* argv[])
{
    argparse::ArgumentParser parser = parse_args(argc, argv);

    bdcpp::curve_data_type ref_curve;
    auto ref_rate = parser.get<std::vector<float>>("ref_rate");
    auto ref_quality = parser.get<std::vector<float>>("ref_quality");
    for (size_t i = 0; i < ref_rate.size(); ++i) {
        ref_curve.push_back(std::make_pair(ref_rate[i], ref_quality[i]));
    }

    bdcpp::curve_data_type test_curve;
    auto test_rate = parser.get<std::vector<float>>("test_rate");
    auto test_quality = parser.get<std::vector<float>>("test_quality");
    for (size_t i = 0; i < test_rate.size(); ++i) {
        test_curve.push_back(std::make_pair(test_rate[i], test_quality[i]));
    }

    const auto bdbr = bdcpp::bdbr(ref_curve, test_curve);
    std::cout << bdbr << "\n";

    return 0;
}
