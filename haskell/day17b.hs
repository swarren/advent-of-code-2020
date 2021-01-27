import qualified Data.Map as Map
import qualified Data.Maybe as Maybe
import qualified Data.Text as T
import qualified Data.Text.IO as TIO

parse :: T.Text -> Map.Map (Int, Int, Int, Int) Char
parse content =
    Map.fromList [((x, y, 0, 0), c) | (y, line) <- zip [0..] lines, (x, c) <- zip [0..] line]
    where
        lines = map T.unpack $ T.lines content

adjacentFilled :: (Int, Int, Int, Int) -> Map.Map (Int, Int, Int, Int) Char -> Int
adjacentFilled (x, y, z, w) grid =
    length $ filter (\c -> c == '#') $ neighborValues
    where
        neighborValues = [Map.findWithDefault '.' coord grid | coord <- neighborCoords]
        neighborCoords = [(x', y', z', w') | x' <- [(x-1)..(x+1)], y' <- [(y-1)..(y+1)], z' <- [(z-1)..(z+1)], w' <- [(w-1)..(w+1)], not (x' == x && y' == y && z' == z && w' == w)]

answer :: Map.Map (Int, Int, Int, Int) Char -> Int
answer grid =
    answer' 6 grid
    where
        answer' 0 grid' = length $ filter (\c -> c == '#') $ Map.elems grid'
        answer' n grid' = answer' (n - 1) newGrid
            where
                xs = map (\(x, y, z, w) -> x) $ Map.keys grid'
                xrange = [(minimum xs)-1..(maximum xs)+1]
                ys = map (\(x, y, z, w) -> y) $ Map.keys grid'
                yrange = [(minimum ys)-1..(maximum ys)+1]
                zs = map (\(x, y, z, w) -> z) $ Map.keys grid'
                zrange = [(minimum zs)-1..(maximum zs)+1]
                ws = map (\(x, y, z, w) -> w) $ Map.keys grid'
                wrange = [(minimum ws)-1..(maximum ws)+1]
                coords = [(x, y, z, w) | x <- xrange, y <- yrange, z <- zrange, w <- wrange]
                newGrid = Map.fromList $ map (\coord -> (coord, newFilled coord)) coords
                newFilled :: (Int, Int, Int, Int) -> Char
                newFilled coord =
                    if Map.findWithDefault '.' coord grid' == '#' then
                        if count == 2 || count == 3 then '#' else '.'
                    else
                        if count == 3 then '#' else '.'
                    where
                        count = adjacentFilled coord grid'

main :: IO ()
main = do
    content <- TIO.readFile "../input/day17.txt"
    let input = parse content
    putStrLn $ show $ answer input
