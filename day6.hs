import qualified Data.List as List
import qualified Data.Set as Set
import qualified Data.Text as T
import qualified Data.Text.IO as TIO

parse :: T.Text -> [[T.Text]]
parse content = map T.lines $ T.splitOn (T.pack "\n\n") content

answer :: [[T.Text]] -> Int
answer content =
    List.foldl' (+) 0 $ map (Set.size . Set.fromList . T.unpack . T.concat) content

main :: IO ()
main = do
    content <- TIO.readFile "day6.txt"
    let input = parse content
    putStrLn $ show $ answer input
