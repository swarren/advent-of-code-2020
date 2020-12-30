import qualified Data.List as List
import qualified Data.Text as T
import qualified Data.Text.IO as TIO

parse :: T.Text -> [Int]
parse content = map (read . T.unpack) $ T.lines $ content

diffs :: [Int] -> [Int]
diffs vals = map (\(a, b) -> b - a) $ zip (0:sortedContent) sortedContent
    where
        sortedContent = List.sort vals

answer :: [Int] -> Int
answer content = diff1 * diff3
    where
        ds = diffs content
        diff1 = length $ filter ((==) 1) ds
        diff3 = 1 + (length $ filter ((==) 3) ds)

main :: IO ()
main = do
    content <- TIO.readFile "day10.txt"
    let input = parse content
    putStrLn $ show $ answer input
