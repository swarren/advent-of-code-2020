import qualified Data.Either as E
import qualified Data.List as L
import qualified Data.Map as M
import qualified Data.Set as S
import qualified Text.ParserCombinators.Parsec as PCPS

eol = PCPS.char '\n'

-- fhmbh jlsqx ... (contains eggs, wheat, sesame)
inputLine = do
    ingredients <- PCPS.many1 $ (PCPS.many1 PCPS.alphaNum) <* (PCPS.string " ")
    PCPS.string "(contains "
    allergens <- PCPS.sepBy (PCPS.many1 PCPS.alphaNum) (PCPS.string ", ")
    PCPS.string ")"
    return (S.fromList ingredients, S.fromList allergens)

inputFile =
    PCPS.many1 (inputLine <* eol) <* PCPS.eof

allergenToIngredientSetOfRecipe :: (S.Set String, S.Set String) -> [(String, S.Set String)]
allergenToIngredientSetOfRecipe (ingredients, allergens) =
    [(allergen, ingredients) | allergen <- S.elems allergens]

allergenToIngredientSetOfRecipes :: [(S.Set String, S.Set String)] -> [(String, S.Set String)]
allergenToIngredientSetOfRecipes recipes =
    L.concatMap allergenToIngredientSetOfRecipe recipes

allergenToIngredientMapOfRecipes :: [(S.Set String, S.Set String)] -> M.Map String (S.Set String)
allergenToIngredientMapOfRecipes recipes =
    M.fromListWith S.intersection $ allergenToIngredientSetOfRecipes recipes

allIngredientsOfRecipes :: [(S.Set String, S.Set String)] -> S.Set String
allIngredientsOfRecipes recipes =
    S.unions $ map fst recipes

ingredientMapIngredients :: M.Map String (S.Set String) -> S.Set String
ingredientMapIngredients ingredientMap =
    S.unions $ M.elems ingredientMap

ingredientMapNonIngredients :: S.Set String -> M.Map String (S.Set String) -> S.Set String
ingredientMapNonIngredients allIngredients ingredientMap =
    S.difference allIngredients $ ingredientMapIngredients ingredientMap

countIngredients :: [(S.Set String, S.Set String)] -> M.Map String Int
countIngredients recipes =
    M.fromListWith (+) $ L.concatMap ingredientTupleOnes recipes
    where
        ingredientTupleOnes (ingredients, allergens) = map (\i -> (i, 1)) $ S.elems ingredients

answer :: [(S.Set String, S.Set String)] -> Int
answer recipes = appearances
    where
        allIngredients = allIngredientsOfRecipes recipes
        ingredientMap = allergenToIngredientMapOfRecipes recipes
        nonAllergenIngredients = ingredientMapNonIngredients allIngredients ingredientMap
        counts = countIngredients recipes
        nonAllergenCounts = M.restrictKeys counts nonAllergenIngredients
        appearances = sum $ M.elems nonAllergenCounts

main :: IO ()
main = do
    let fn = "day21.txt"
    inputText <- readFile fn
    let input = E.fromRight [] $ PCPS.parse inputFile fn inputText
    putStrLn $ show $ answer $ input
