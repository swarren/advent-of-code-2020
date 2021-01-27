import qualified Data.Map as Map
import qualified Data.Text as T
import qualified Data.Text.IO as TIO

parse :: T.Text -> Map.Map (Int, Int) Char
parse content = grid
    where
        lines = map T.unpack $ T.lines content
        gridMain = Map.fromList [((x, y), c) | (y, line) <- zip [0..] lines, (x, c) <- zip [0..] line]
        ysize = length lines
        xsize = length $ head lines
        baseGrid = Map.fromList [((x, y), 'B') | x <- [-1..xsize], y <- [-1..ysize]]
        grid = Map.union gridMain baseGrid

seenFilled :: (Int, Int) -> Map.Map (Int, Int) Char -> Int
seenFilled (x, y) grid =
    length $ filter (\c -> c == '#') $ firstSeen
    where
        neighborDirs = [(dx, dy) | dx <- [-1..1], dy <- [-1..1], not (dx == 0 && dy == 0)]
        neighborRay (dx, dy) = [(x + (n * dx), y + (n * dy)) | n <- [1..]]
        neighborRayValues dir = map (\(x', y') -> Map.findWithDefault '.' (x', y') grid) $ neighborRay dir
        neighborRayFirst dir = head $ filter (\c -> c /= '.') $ neighborRayValues dir
        firstSeen = [neighborRayFirst dir | dir <- neighborDirs]

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
            if c == 'B' then
                'B'
            else if c == '.' then
                '.'
            else if c == 'L' then
                if count == 0 then '#' else 'L'
            else
                if count >= 5 then 'L' else '#'
            where
                count = seenFilled (x, y) grid

main :: IO ()
main = do
    content <- TIO.readFile "../input/day11.txt"
    let input = parse content
    putStrLn $ show $ answer input
