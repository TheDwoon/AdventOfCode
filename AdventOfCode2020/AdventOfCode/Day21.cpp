#include "Day21.h"
#include <algorithm>
#include <map>
#include <set>

std::vector<Food> Day21::parseInput(std::string &input) {
    std::vector<Food> parsed;
    std::stringstream stream(input);

    for (std::string line; std::getline(stream, line);) {
        Food food;
        std::stringstream lineStream(line);

        std::string token;
        while (std::getline(lineStream, token, ' ') && token.find('(') == std::string::npos)
            food.ingredients.push_back(token);

        while (std::getline(lineStream, token, ' ')) {
            if (token[token.size() - 1] == ',' || token[token.size() - 1] == ')')
                token = token.substr(0, token.size() - 1);

            food.allergen.push_back(token);
        }

        parsed.push_back(food);
    }

    return parsed;
}

Food intersectFood(const Food &first, const Food &second) {
    Food intersected;
    for (const std::string &ingredient : first.ingredients)
        if (std::find(second.ingredients.begin(), second.ingredients.end(), ingredient) != second.ingredients.end())
            intersected.ingredients.push_back(ingredient);

    for (const std::string &allergen : first.allergen)
        if (std::find(second.allergen.begin(), second.allergen.end(), allergen) != second.allergen.end())
            intersected.allergen.push_back(allergen);

    return intersected;
}

void filterKnown(std::vector<std::string> &ingredients, std::vector<std::string> &allergen, const std::map<std::string, std::string> &knownAllergens) {
    for (auto it = knownAllergens.begin(); it != knownAllergens.end(); ++it) {
        auto allergenIt = std::find(allergen.begin(), allergen.end(), it->first);
        if (allergenIt != allergen.end())
            allergen.erase(allergenIt);

        auto ingredientsIt = std::find(ingredients.begin(), ingredients.end(), it->second);
        if (ingredientsIt != ingredients.end())
            ingredients.erase(ingredientsIt);
    }
}

std::map<std::string, std::string> computeKnownAllergenes(const std::vector<Food>& input) {
    std::map<std::string, std::string> knownAllergens;
    std::set<std::string> allergens;

    // build set of allergens
    for (const Food &food : input) {
        for (const std::string &allergen : food.allergen) {
            allergens.insert(allergen);
        }
    }

    // iterate over allergens and remove them if possible
    while (!allergens.empty()) {
        for (std::string allergen : allergens) {
            // intersect food with the given allergen
            auto it = std::find_if(input.begin(), input.end(), [&allergen] (const Food &f) {
                return std::find(f.allergen.begin(), f.allergen.end(), allergen) != f.allergen.end();
            });
            if (it == input.end())
                continue;

            Food intersect = *it;
            while (it != input.end()) {
                intersect = intersectFood(intersect, *it);

                it = std::find_if(it + 1, input.end(), [&allergen] (const Food &f) {
                    return std::find(f.allergen.begin(), f.allergen.end(), allergen) != f.allergen.end();
                });
            }

            filterKnown(intersect.ingredients, intersect.allergen, knownAllergens);

            if (intersect.ingredients.size() == 1)
                knownAllergens[intersect.allergen[0]] = intersect.ingredients[0];
        }

        for (auto it = knownAllergens.begin(); it != knownAllergens.end(); ++it) {
            auto allergenIt = allergens.find(it->first);
            if (allergenIt != allergens.end())
                allergens.erase(allergenIt);
        }
    }

    return knownAllergens;
}

std::string Day21::runPart1(std::vector<Food> &input) {
    std::stringstream output;
    std::map<std::string, std::string> knownAllergens = computeKnownAllergenes(input);

    std::map<std::string, std::string> knownIngredients;
    for (auto it = knownAllergens.begin(); it != knownAllergens.end(); ++it)
        knownIngredients[it->second] = it->first;

    int nonAllergenCount = 0;
    for (Food& food : input) {
        for (std::string &ingredient : food.ingredients) {
            nonAllergenCount += (knownIngredients.find(ingredient) == knownIngredients.end());
        }
    }

    output << nonAllergenCount;
    return output.str();
}

std::string Day21::runPart2(std::vector<Food> &input) {
    std::stringstream output;
    std::map<std::string, std::string> knownAllergens = computeKnownAllergenes(input);

    std::vector<std::string> allergens;
    for (auto it = knownAllergens.begin(); it != knownAllergens.end(); ++it)
        allergens.push_back(it->first);

    std::sort(allergens.begin(), allergens.end());

    for (int i = 0; i < allergens.size(); i++) {
        output << knownAllergens[allergens[i]];
        if (i < allergens.size() - 1)
            output << ',';
    }

    return output.str();
}
