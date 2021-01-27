import qualified Data.List as List
import qualified Data.Text as T
import qualified Data.Text.IO as TIO

parse :: T.Text -> [Int]
parse content = map (read . T.unpack) $ T.lines $ content

diffs :: [Int] -> [Int]
diffs vals = map (\(a, b) -> b - a) $ zip (0:sortedContent) sortedContent
    where
        sortedContent = List.sort vals

collapses :: Int -> Int
collapses l
    | l < 2     = 1
    | otherwise = 1 + r1 + r2
    where
        r1 = List.foldl' (+) 0 [collapses (l - i - 2) | i <- [0..(l-2)]]
        r2 = List.foldl' (+) 0 [collapses (l - i - 3) | i <- [0..(l-3)]]

answer :: [Int] -> Int
answer content = List.foldl' (*) 1 combo1s
    where
        ds = diffs content
        gs = List.group ds
        g1s = filter (\l -> 1 `elem` l) gs
        combo1s = map (collapses .length) g1s

main :: IO ()
main = do
    content <- TIO.readFile "../input/day10.txt"
    let input = parse content
    putStrLn $ show $ answer input
