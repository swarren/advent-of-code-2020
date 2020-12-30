import qualified Data.Text as T
import qualified Data.Text.IO as TIO

plen :: Int
plen = 25

parse :: T.Text -> [Int]
parse content = map (read . T.unpack) $ T.lines $ content

isValid :: [Int] -> Int -> Bool
isValid prefix value =
    or [True | x <- prefix, y <- prefix, x /= y, x + y == value]

findInvalid :: [Int] -> [Int] -> Int
findInvalid prefix (value:values) =
    if not $ isValid prefix value then
        value
    else
        findInvalid ((tail prefix) ++ [value]) values

answer :: [Int] -> Int
answer content = findInvalid prefix values
    where
        prefix = take plen content
        values = drop plen content

main :: IO ()
main = do
    content <- TIO.readFile "day9.txt"
    let input = parse content
    putStrLn $ show $ answer input
