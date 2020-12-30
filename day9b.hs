import qualified Data.Text as T
import qualified Data.Text.IO as TIO

plen :: Int
plen = 25

parse :: T.Text -> [Int]
parse content = map (read . T.unpack) $ T.lines $ content

isValid :: [Int] -> Int -> Bool
isValid prefix value =
    or [True | x <- prefix, y <- prefix, x /= y, x + y == value]

findInvalid :: [Int] -> [Int] -> Int -> (Int, Int)
findInvalid prefix (value:values) i =
    if not $ isValid prefix value then
        (i, value)
    else
        findInvalid ((tail prefix) ++ [value]) values (i + 1)

findSumTo :: [Int] -> Int -> [Int]
findSumTo vals val = head validLists
    where
        nvals = length vals
        lists = [take n $ drop i vals | n <- [2..nvals], i <- [2..(nvals - n)]]
        validLists = filter (\l -> sum l == val) lists

answer :: [Int] -> Int
answer content = sumToMin + sumToMax
    where
        prefix = take plen content
        values = drop plen content
        (invalidPos, invalidVal) = findInvalid prefix values 0
        sumTo = findSumTo (take (plen + invalidPos) content) invalidVal
        sumToMin = minimum sumTo
        sumToMax = maximum sumTo

main :: IO ()
main = do
    content <- TIO.readFile "day9.txt"
    let input = parse content
    putStrLn $ show $ answer input
