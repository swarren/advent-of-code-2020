import qualified Data.Text as T
import qualified Data.Text.IO as TIO

parse :: T.Text -> [T.Text]
parse content = T.lines $ content

treeCount :: Int -> Int -> [T.Text] -> Int
treeCount col dcol [] = 0
treeCount col dcol (row:rows) = thisCount + remainingCount
    where
        ix = col `mod` (T.length row)
        thisCount = if (T.index row ix) == '#' then 1 else 0
        remainingCount = treeCount (col + dcol) dcol rows

answer :: [T.Text] -> Int
answer content = treeCount 0 3 content

main :: IO ()
main = do
    content <- TIO.readFile "day3.txt"
    let input = parse content
    putStrLn $ show $ answer input
