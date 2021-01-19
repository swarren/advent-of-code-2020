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

assign :: M.Map String (S.Set String) -> [(String, String)]
assign ingredientMap = assign' ingredientMap []
    where
        assign' :: M.Map String (S.Set String) -> [(String, String)] -> [(String, String)]
        assign' ingredientMap assignments =
            if null ingredientMap then assignments else assign' ingredientMap'' assignments'
            where
                known = M.filter (\is -> length is == 1) ingredientMap
                knownAllergens = M.keysSet known
                knownIngredients = S.unions $ M.elems known
                ingredientMap' = M.withoutKeys ingredientMap $ knownAllergens
                ingredientMap'' = M.map (\is -> S.difference is knownIngredients) ingredientMap'
                assignments' = (map (\(a, is) -> (a, head $ S.elems is)) $ M.assocs known) ++ assignments

answer :: [(S.Set String, S.Set String)] -> String
answer recipes = dangerousIngredientsList
    where
        ingredientMap = allergenToIngredientMapOfRecipes recipes
        assign' = assign ingredientMap
        sortedAssign = L.sort assign'
        dangerousIngredients = map snd sortedAssign
        dangerousIngredientsList = L.concat $ L.intersperse "," dangerousIngredients

main :: IO ()
main = do
    let fn = "day21.txt"
    inputText <- readFile fn
    let input = E.fromRight [] $ PCPS.parse inputFile fn inputText
    putStrLn $ answer $ input
