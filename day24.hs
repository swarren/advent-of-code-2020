import qualified Data.List as L
import qualified Data.Set as S
import qualified Data.Text as T

parse :: String -> [String]
parse content = lines content

pos :: String -> (Int, Int)
pos s = pos' (0, 0) s
    where
        pos' :: (Int, Int) -> [Char] -> (Int, Int)
        pos' (x, y) []     = (x, y)
        pos' (x, y) ('n':'e':s) = pos' (x + 1, y + 1) s
        pos' (x, y) ('s':'e':s) = pos' (x + 1, y - 1) s
        pos' (x, y) ('n':'w':s) = pos' (x - 1, y + 1) s
        pos' (x, y) ('s':'w':s) = pos' (x - 1, y - 1) s
        pos' (x, y) ('e':s)     = pos' (x + 2, y)     s
        pos' (x, y) ('w':s)     = pos' (x - 2, y)     s

answer :: [String] -> Int
answer lines = 
    length $ L.foldl' updateFloor S.empty $ map pos lines
    where
        updateFloor :: S.Set (Int, Int) -> (Int, Int) -> S.Set (Int, Int)
        updateFloor s t =
            if t `elem` s then
                S.delete t s
            else
                S.insert t s

main :: IO ()
main = do
    content <- readFile "day24.txt"
    let input = parse content
    putStrLn $ show $ answer input
