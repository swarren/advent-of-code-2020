#!/usr/bin/env python3

with open("../input/day21.txt") as f:
    data = f.read().splitlines()

recipes = []
all_ingredients = {}
all_allergens = {}
for l in data:
    ingredients_l, allergens_l = l.split('(contains ')
    ingredients = ingredients_l.split()
    allergens = allergens_l[:-1].split(', ')
    recipes.append((ingredients, allergens))
    for ingredient in ingredients:
        all_ingredients[ingredient] = True
    for allergen in allergens:
        all_allergens[allergen] = True
all_ingredients = all_ingredients.keys()
all_allergens = all_allergens.keys()
#print('recipes', recipes)
#print('all_ingredients', all_ingredients)
#print('all_allergens', all_allergens)

ingredient_allergen_counts = {}
for ingredients, allergens in recipes:
    for ingredient in ingredients:
        for allergen in allergens:
            ingredient_allergen_counts[ingredient] = ingredient_allergen_counts.get(ingredient, {})
            ingredient_allergen_counts[ingredient][allergen] = ingredient_allergen_counts[ingredient].get(allergen, 0)
            ingredient_allergen_counts[ingredient][allergen] += 1
#print('ingredient_allergen_counts', ingredient_allergen_counts)

recipe_allergen_counts = {}
for recipe, allergens in recipes:
    for allergen in allergens:
        recipe_allergen_counts[allergen] = recipe_allergen_counts.get(allergen, 0)
        recipe_allergen_counts[allergen] += 1
#print('recipe_allergen_counts', recipe_allergen_counts)

ingredients_no_allergens = []
for ingredient in all_ingredients:
    possible_allergens = 0
    for allergen, count in ingredient_allergen_counts[ingredient].items():
        if count == recipe_allergen_counts[allergen]:
            possible_allergens += 1
    if not possible_allergens:
        ingredients_no_allergens.append(ingredient)
#print('ingredients_no_allergens', ingredients_no_allergens)

ingredient_possible_allergens = {}
for ingredient in all_ingredients:
    if ingredient in ingredients_no_allergens:
        continue
    possible_allergens = []
    for allergen, count in ingredient_allergen_counts[ingredient].items():
        if count == recipe_allergen_counts[allergen]:
            possible_allergens.append(allergen)
    ingredient_possible_allergens[ingredient] = possible_allergens
#print('ingredient_possible_allergens', ingredient_possible_allergens)

ingredient_allergen = []
while len(ingredient_allergen) != len(all_ingredients) - len(ingredients_no_allergens):
    for ingredient, possible_allergens in ingredient_possible_allergens.items():
        if len(possible_allergens) == 1:
            this_allergen = possible_allergens[0]
            ingredient_allergen.append((this_allergen, ingredient))
            for _, possible_allergens in ingredient_possible_allergens.items():
                if this_allergen in possible_allergens:
                    possible_allergens.remove(this_allergen)
            break
#print(ingredient_allergen)

print(','.join(map(lambda ia: ia[1], sorted(ingredient_allergen))))
