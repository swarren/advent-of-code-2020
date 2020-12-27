import qualified Data.Text as T
import qualified Data.Text.IO as TIO

parse :: T.Text -> [T.Text]
parse content = T.lines $ content

treeCount :: Int -> Int -> Int -> [T.Text] -> Int
treeCount col dcol drow [] = 0
treeCount col dcol drow rows = thisCount + remainingCount
    where
        row = head rows
        ix = col `mod` (T.length row)
        thisCount = if (T.index row ix) == '#' then 1 else 0
        rows' = drop drow rows
        remainingCount = treeCount (col + dcol) dcol drow rows'

slopes :: [(Int, Int)]
slopes = [
    (1, 1),
    (3, 1),
    (5, 1),
    (7, 1),
    (1, 2)
    ]

answer :: [T.Text] -> Int
answer content = foldl (*) 1 [treeCount 0 dcol drow content | (dcol, drow) <- slopes]

main :: IO ()
main = do
    content <- TIO.readFile "day3.txt"
    let input = parse content
    putStrLn $ show $ answer input
