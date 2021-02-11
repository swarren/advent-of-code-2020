#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>
#include <experimental/vector>

using Ingredient = std::string;
using Ingredients = std::vector<Ingredient>;
using Allergen = std::string;
using Allergens = std::vector<Allergen>;
struct Food {
    Ingredients ingredients;
    Allergens allergens;
};
using Input = std::vector<Food>;

Input readParseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input input;

    std::string line;
    // a b c (contains x, y, z)
    while (std::getline(file, line)) {
        auto len = line.length();
        Food &food = input.emplace_back();
        auto *list = &food.ingredients;
        std::string::size_type parsePos = 0;

        while (true) {
            auto spacePos = line.find(' ', parsePos);
            if (spacePos == std::string::npos)
                spacePos = len;
            auto tokenEnd = spacePos;
            switch (line[tokenEnd - 1]) {
            case ',':
            case ')':
                tokenEnd--;
                break;
            default:
                break;
            }
            list->emplace_back(line.substr(parsePos, tokenEnd - parsePos));
            if (list->back() == "(contains") {
                list->pop_back();
                list = &food.allergens;
            }
            if (spacePos == len)
                break;
            parsePos = spacePos + 1;
        }
    }
    file.close();

    return input;
}

auto answer(const Input &input) {
    using IngredientCounts = std::map<Ingredient, int>;
    using AllergenCounts = std::map<Allergen, int>; 
    IngredientCounts ingredientAppearances;
    std::map<Ingredient, AllergenCounts> ingredientAllergenCoappearances;
    AllergenCounts allergenAppearances;

    for (const auto &food : input) {
        for (const auto &ingredient : food.ingredients) {
            ingredientAppearances[ingredient]++;
            auto [allergenCoappearancesIt, inserted] =
                ingredientAllergenCoappearances.emplace(std::make_pair(ingredient, AllergenCounts{}));
            auto &allergenCoappearances = allergenCoappearancesIt->second;
            for (const auto &allergen : food.allergens)
                allergenCoappearances[allergen]++;
        }
        for (const auto &allergen : food.allergens)
            allergenAppearances[allergen]++;
    }

    using AllergenList = std::vector<Allergen>;
    std::map<Ingredient, AllergenList> ingredientPossibleAllergens;
    for (const auto &[ingredient, countI] : ingredientAppearances) {
        const auto &allergenCoappearances = ingredientAllergenCoappearances[ingredient];
        for (const auto &[allergen, countIA] : allergenCoappearances) {
            int countA = allergenAppearances[allergen];
            if (countIA == countA)
                ingredientPossibleAllergens[ingredient].push_back(allergen);
        }
        if (!ingredientPossibleAllergens[ingredient].size())
            ingredientPossibleAllergens.erase(ingredient);
    }

    std::map<Allergen, Ingredient> allergenToIngredient;
    while (ingredientPossibleAllergens.size()) {
        auto hasOneAllergen = [](const std::pair<Ingredient, AllergenList> ias) { return ias.second.size() == 1; };
        auto pos = std::find_if(ingredientPossibleAllergens.cbegin(), ingredientPossibleAllergens.cend(), hasOneAllergen);
        const auto &[ingredient, allergens] = *pos;
        const auto &allergen = allergens[0];
        allergenToIngredient[allergen] = ingredient;
        for (auto &[ingredient2, allergens2] : ingredientPossibleAllergens)
            std::experimental::erase(allergens2, allergen);
        ingredientPossibleAllergens.erase(pos);
    }

    std::string answer;
    bool first = true;
    for (const auto &[allergen, ingredient] : allergenToIngredient) {
        if (first)
            first = false;
        else
            answer += ',';
        answer = answer + ingredient;
    }

    return answer;
}

int main(void) {
    std::cout << answer(readParseInput("../input/day21.txt")) << '\n';
    return 0;
}
