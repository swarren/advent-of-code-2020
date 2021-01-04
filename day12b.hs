import qualified Data.Complex as C
import qualified Data.Text as T
import qualified Data.Text.IO as TIO

parse :: T.Text -> [(Char, Int)]
parse content =
    [(head l, read $ tail l) | l <- lines]
    where
        lines = map T.unpack $ T.lines content

move :: (C.Complex Float, C.Complex Float) -> (Char, Float) -> (C.Complex Float, C.Complex Float)
move (pos, waypoint) ('N', param) = (pos, waypoint + (0 C.:+ param))
move (pos, waypoint) ('S', param) = (pos, waypoint - (0 C.:+ param))
move (pos, waypoint) ('E', param) = (pos, waypoint + (param C.:+ 0))
move (pos, waypoint) ('W', param) = (pos, waypoint - (param C.:+ 0))
move (pos, waypoint) ('L', 0) = (pos, waypoint)
move (pos, waypoint) ('L', param) = move (pos, waypoint * (0 C.:+ 1)) ('L', param - 90)
move (pos, waypoint) ('R', 0) = (pos, waypoint)
move (pos, waypoint) ('R', param) = move (pos, waypoint * (0 C.:+ (-1))) ('R', param - 90)
move (pos, waypoint) ('F', param) = (pos + (waypoint * (param C.:+ 0)), waypoint)

answer :: [(Char, Int)] -> Int
answer moves = x + y
    where
        finalState = foldl move (0 C.:+ 0, 10 C.:+ 1) $ map (\(m, i) -> (m, fromIntegral i)) moves
        finalPos = fst $ finalState
        x = abs $ round $ C.realPart finalPos
        y = abs $ round $ C.imagPart finalPos

main :: IO ()
main = do
    content <- TIO.readFile "day12.txt"
    let input = parse content
    putStrLn $ show $ answer input
