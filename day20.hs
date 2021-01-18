import qualified Data.Either as E
import qualified Data.List as L
import qualified Data.Map as M
import qualified Text.ParserCombinators.Parsec as PCPS

eol = PCPS.char '\n'

-- Tile 2903:
inputTileHeader = do
    PCPS.string "Tile "
    tileNumStr <- PCPS.many1 PCPS.digit
    PCPS.char ':'
    eol
    return (read tileNumStr :: Int)

-- ..#..#....
inputTileRow = do
    row <- PCPS.many1 $ PCPS.oneOf "#."
    eol
    return row

inputTile = do
    tileNum <- inputTileHeader
    tileRows <- PCPS.many inputTileRow
    return (tileNum, tileRows)

inputFile = do
    tiles <- PCPS.sepBy inputTile eol
    PCPS.eof
    return tiles

rowToInt :: String -> Int
rowToInt s =
    L.foldl' (\v c -> v * 2 + c) 0 $ map c2i s
    where
        c2i c = if c == '#' then 1 else 0

tileBorders :: [String] -> [Int]
tileBorders tileRows =
    map rowToInt [t, tr, r, rr, b, br, l, lr]
    where
        t = head tileRows
        tr = reverse t
        r = [last row | row <- tileRows]
        rr = reverse r
        b = last tileRows
        br = reverse b
        l = [head row | row <- tileRows]
        lr = reverse l

tileBordersByTile :: [(Int, [String])] -> [(Int, [Int])]
tileBordersByTile tiles =
    map (\(tileNum, tileRows) -> (tileNum, tileBorders tileRows)) tiles

borderToTile :: [(Int, [String])] -> M.Map Int [Int]
borderToTile tiles =
    M.fromListWith (++) $ foldl1 (++) (map borderToTile' $ tileBordersByTile tiles)
    where
        borderToTile' (tileNum, borders) = map (\b -> (b, [tileNum])) borders

edgeBorders :: [(Int, [String])] -> [Int]
edgeBorders tiles =
    M.keys $ M.filter (\tiles -> length tiles == 1) (borderToTile tiles)

tileToNumEdges :: [(Int, [String])] -> [(Int, Int)]
tileToNumEdges tiles =
    map tileNumEdges tiles
    where
        edgeBorders' = edgeBorders tiles
        tileNumEdges (tileNum, tileRows) = (tileNum, tileRowsNumEdges tileRows)
        tileRowsNumEdges tileRows = (length $ L.intersect (tileBorders tileRows) edgeBorders') `div` 2

cornerTiles :: [(Int, [String])] -> [Int]
cornerTiles tiles =
    map fst $ L.filter isCorner $ tileToNumEdges tiles
    where
        isCorner (tileNum, numEdges) = numEdges == 2

answer tiles = product $ cornerTiles tiles

main :: IO ()
main = do
    let fn = "day20.txt"
    inputText <- readFile fn
    let input = E.fromRight [] $ PCPS.parse inputFile fn inputText
    putStrLn $ show $ answer $ input
