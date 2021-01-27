import qualified Data.List as List
import qualified Data.Set as Set
import qualified Data.Text as T
import qualified Data.Text.IO as TIO

parse :: T.Text -> [[T.Text]]
parse content = map T.lines $ T.splitOn (T.pack "\n\n") content

answerGroup :: [T.Text] -> Int
answerGroup g =
    Set.size $ List.foldl Set.intersection (head answers) $ tail answers
    where
        answers = map (Set.fromList . T.unpack) g

answer :: [[T.Text]] -> Int
answer content =
    List.foldl' (+) 0 $ map answerGroup content

main :: IO ()
main = do
    content <- TIO.readFile "../input/day6.txt"
    let input = parse content
    putStrLn $ show $ answer input
