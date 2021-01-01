import qualified Data.Map as Map
import qualified Data.Text as T
import qualified Data.Text.IO as TIO

parse :: T.Text -> Map.Map (Int, Int) Char
parse content =
    Map.fromList [((x, y), c) | (y, line) <- zip [0..] lines, (x, c) <- zip [0..] line, c /= '.']
    where
        lines = map T.unpack $ T.lines content

adjacentFilled :: (Int, Int) -> Map.Map (Int, Int) Char -> Int
adjacentFilled (x, y) grid =
    length $ filter (\c -> c == '#') $ neighborValues
    where
        neighborValues = [Map.findWithDefault '.' (x', y') grid | (x', y') <- neighborCoords]
        neighborCoords = [(x', y') | x' <- [(x-1)..(x+1)], y' <- [(y-1)..(y+1)], not (x' == x && y' == y)]

answer :: Map.Map (Int, Int) Char -> Int
answer grid =
    if newGrid == grid then
        length $ filter (\c -> c == '#') $ Map.elems grid
    else
        answer newGrid
    where
        newGrid = Map.mapWithKey newFilled grid
        newFilled :: (Int, Int) -> Char -> Char
        newFilled (x, y) c =
            if c == 'L' then
                if count == 0 then '#' else 'L'
            else
                if count >= 4 then 'L' else '#'
            where
                count = adjacentFilled (x, y) grid

main :: IO ()
main = do
    content <- TIO.readFile "day11.txt"
    let input = parse content
    putStrLn $ show $ answer input
