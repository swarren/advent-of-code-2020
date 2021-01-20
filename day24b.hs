import qualified Data.List as L
import qualified Data.Maybe as Maybe
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

initFloor :: [String] -> S.Set (Int, Int)
initFloor lines = 
    L.foldl' updateFloor S.empty $ map pos lines
    where
        updateFloor :: S.Set (Int, Int) -> (Int, Int) -> S.Set (Int, Int)
        updateFloor s t =
            if t `elem` s then
                S.delete t s
            else
                S.insert t s

neighbourCoords :: (Int, Int) -> [(Int, Int)]
neighbourCoords (x, y) =
    [(x - 2, y), (x - 1, y + 1), (x + 1, y + 1), (x + 2, y), (x + 1, y - 1), (x - 1, y - 1)]

iterNextDay :: S.Set (Int, Int) -> S.Set (Int, Int) -> S.Set (Int, Int) -> S.Set (Int, Int) -> S.Set (Int, Int)
iterNextDay curDay nextDay coordsToVisit coordsVisited
    | S.null coordsToVisit = nextDay
    | otherwise            = iterNextDay curDay nextDay' coordsToVisit' coordsVisited'
    where
        coord = Maybe.fromJust $ S.lookupMin coordsToVisit
        neighbourVals = map (\c -> (c, c `elem` curDay)) $ neighbourCoords coord
        numBlackNeighbours = length $ filter (\(c, v) -> v) neighbourVals
        whiteNeighbours = S.fromList $ map fst $ filter (\(c, v) -> not v) neighbourVals
        curIsBlack = coord `elem` curDay
        unvisitedWhiteNeighbours = if curIsBlack then S.difference whiteNeighbours coordsVisited else S.empty
        newIsBlack =
            if curIsBlack then
                numBlackNeighbours >= 1 && numBlackNeighbours <= 2
            else
                numBlackNeighbours == 2
        nextDay' = if newIsBlack then S.insert coord nextDay else nextDay
        coordsToVisit' = S.delete coord $ S.union coordsToVisit unvisitedWhiteNeighbours
        coordsVisited' = S.insert coord coordsVisited

nextDay :: S.Set (Int, Int) -> Int -> S.Set (Int, Int)
nextDay curDay _ = iterNextDay curDay S.empty curDay S.empty

answer :: [String] -> Int
answer lines = length $ finalFloor
    where
        initFloor' = initFloor lines
        finalFloor = L.foldl' nextDay initFloor' [1..100]

main :: IO ()
main = do
    content <- readFile "day24.txt"
    let input = parse content
    putStrLn $ show $ answer input
