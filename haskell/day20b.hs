import Data.Bits
import qualified Data.Either as E
import qualified Data.List as L
import qualified Data.Map as M
import qualified Data.Set as S
import qualified Text.ParserCombinators.Parsec as PCPS

data Tile = Tile {
    tileNum :: Int,
    tileRows :: [String],
    tileBorderIds :: [Int]
}

instance Show Tile where
    show tile = "Tile " ++ (show $ tileNum tile) ++ " " ++ (show $ tileBorderIds tile) ++ "\n" ++ (L.unlines $ tileRows tile)

topBorderId tile = (tileBorderIds tile) !! 0

rightBorderId tile = (tileBorderIds tile) !! 1

bottomBorderId tile = (tileBorderIds tile) !! 2

leftBorderId tile = (tileBorderIds tile) !! 3

rowToInt :: Num a => String -> a
rowToInt s =
    L.foldl' (\v c -> v * 2 + c) 0 $ map c2i s
    where
        c2i c = if c == '#' then 1 else 0

tileRowsToBorderIds :: [String] -> [Int]
tileRowsToBorderIds tileRows =
    map rowToInt $ [t, r, b, l]
    where
        t = head tileRows
        r = [last row | row <- tileRows]
        b = last tileRows
        l = [head row | row <- tileRows]

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
    return $ Tile tileNum tileRows (tileRowsToBorderIds tileRows)

inputFile = do
    tiles <- PCPS.sepBy inputTile eol
    PCPS.eof
    return tiles

rotateRows :: [String] -> [String]
rotateRows rows = rows'
    where
        prevH = length rows
        prevW = length $ head rows
        rows' = [[(rows !! (prevH - newX - 1)) !! newY | newX <- [0..prevH-1]] | newY <- [0..prevW-1]]

hflipRows :: [String] -> [String]
hflipRows rows = rows'
    where
        rows' = map reverse rows

allOrientationsRows :: [String] -> [[String]]
allOrientationsRows rows = [r0, r0h, r90, r90h, r180, r180h, r270, r270h]
    where
        r0 = rows
        r0h = hflipRows r0
        r90 = rotateRows r0
        r90h = hflipRows r90
        r180 = rotateRows r90
        r180h = hflipRows r180
        r270 = rotateRows r180
        r270h = hflipRows r270

allOrientationsTile :: Tile -> [Tile]
allOrientationsTile tile =
    map genTile $ allOrientationsRows $ tileRows tile
    where
        genTile rows = Tile (tileNum tile) rows (tileRowsToBorderIds rows)

allOrientationsTiles :: [Tile] -> [Tile]
allOrientationsTiles tiles =
    concat $ map (allOrientationsTile) tiles

borderIdToTileNums :: [Tile] -> M.Map Int (S.Set Int)
borderIdToTileNums tiles =
    M.fromListWith S.union $ L.concat (map borderIdToTile' tiles)
    where
        borderIdToTile' tile = map (\b -> (b, S.singleton (tileNum tile))) (tileBorderIds tile)

edgeBorderIds :: M.Map Int (S.Set Int) -> [Int]
edgeBorderIds borderIdToTileNums =
    M.keys $ M.filter (\tiles -> length tiles == 1) borderIdToTileNums

tileNumToEdgeCount :: [Tile] -> [Int] -> M.Map Int Int
tileNumToEdgeCount tiles edgeBorderIds =
    M.fromList $ map (\tile -> (tileNum tile, tileNumToEdgeCount' tile)) tiles
    where
        tileNumToEdgeCount' tile = length $ L.intersect (tileBorderIds tile) edgeBorderIds

cornerTiles :: M.Map Int Int -> [Int]
cornerTiles tileNumToEdgeCount =
    M.keys $ M.filter (==2) $ tileNumToEdgeCount

badTile :: Tile
badTile = Tile (-1) ["."] []

notTile :: Tile -> Tile -> Bool
notTile t1 t2 = (tileNum t1) /= (tileNum t2)

solveGrid :: [Tile] -> [Int] -> Tile -> [[Tile]]
solveGrid tiles edgeBorderIds botRightTile =
    solveGrid' unplacedTiles edgeBorderIds (L.repeat edgeBorderIds)
        [[topBorderId botRightTile]] [leftBorderId botRightTile] [[botRightTile]]
    where
        unplacedTiles = L.filter (notTile botRightTile) tiles

solveGrid' :: [Tile] -> [Int] -> [[Int]] -> [[Int]] -> [Int] -> [[Tile]] -> [[Tile]]
solveGrid' unplacedTiles edgeBorderIds belowConstraints nextBelowConstraints rightConstraint result =
    if null candidateTiles then
        result'
    else
        if isTopEdge && isLeftEdge then
            result'
        else
            solveGrid' unplacedTiles' edgeBorderIds belowConstraints' nextBelowConstraints'' rightConstraint' result''
    where
        belowConstraint :: [Int]
        belowConstraint = head belowConstraints

        candidateTiles :: [Tile]
        candidateTiles = L.filter constraintsMatch unplacedTiles
            where
                constraintsMatch candidateTile =
                    ((bottomBorderId candidateTile) `elem` belowConstraint) &&
                    ((rightBorderId candidateTile) `elem` rightConstraint)

        tile :: Tile
        tile = if null candidateTiles then badTile else head candidateTiles

        unplacedTiles' :: [Tile]
        unplacedTiles' = L.filter (notTile tile) unplacedTiles

        isTopEdge :: Bool
        isTopEdge = (topBorderId tile) `elem` edgeBorderIds

        isLeftEdge :: Bool
        isLeftEdge = (leftBorderId tile) `elem` edgeBorderIds

        rightConstraint' :: [Int]
        rightConstraint' = if isLeftEdge then edgeBorderIds else [leftBorderId tile]

        nextBelowConstraints' :: [[Int]]
        nextBelowConstraints' = [topBorderId tile]:nextBelowConstraints

        belowConstraints' :: [[Int]]
        belowConstraints' = if isLeftEdge then (L.reverse nextBelowConstraints') else (tail belowConstraints)

        nextBelowConstraints'' :: [[Int]]
        nextBelowConstraints'' = if isLeftEdge then [] else nextBelowConstraints'

        result' :: [[Tile]]
        result' = (tile : (head result)) : (tail result)

        result'' :: [[Tile]]
        result'' = if isLeftEdge then []:result' else result'

stitchedGrid :: [[Tile]] -> [String]
stitchedGrid grid = gridLines
    where
        numGridRows :: Int
        numGridRows = length grid

        numGridCols :: Int
        numGridCols = length $ head grid

        numTileRows :: Int
        numTileRows = length $ tileRows $ head $ head grid

        rowTexts :: Int -> Int -> [String]
        rowTexts gridRow tileRow = map (\t -> (tileRows t) !! tileRow) (grid !! gridRow)

        rowText :: Int -> Int -> String
        rowText gridRow tileRow = concat $ map (\s -> init $ tail s) (rowTexts gridRow tileRow)

        gridLines :: [String]
        gridLines = [rowText gridRow tileRow | gridRow <- [0..numGridRows-1], tileRow <- [1..numTileRows-2]]

monsterText =
    [
        "                  # ",
        "#    ##    ##    ###",
        " #  #  #  #  #  #   "
    ]

intMonster :: [Integer]
intMonster = map rowToInt monsterText

intMonsterRows :: Int
intMonsterRows = length monsterText

intMonsterCols :: Int
intMonsterCols = length $ head monsterText

countMonsters :: [String] -> Int
countMonsters grid = monsterCount
    where
        numGridRows = length grid
        numGridCols = length $ head grid
        intGrid :: [Integer]
        intGrid = map rowToInt grid
        checkAtColMax = numGridCols - intMonsterCols + 1
        checkAtRowMax = numGridRows - intMonsterRows + 1
        checkAtCols = [0..checkAtColMax]
        checkAtRows = [0..checkAtRowMax]
        monsterAt col row =        
            L.and [
                (shiftR (intGrid !! (row + monsterRow)) col) .&. (intMonster !! monsterRow) == (intMonster !! monsterRow)
                | monsterRow <- [0..intMonsterRows-1]
            ]
        monsterAts = [monsterAt col row | row <- checkAtRows, col <- checkAtCols]
        monsterCount = length $ filter (== True) $ monsterAts

hashCount :: [String] -> Int
hashCount rows =
    sum $ map (length . filter (=='#')) rows

answer :: [Tile] -> Int
answer tiles = result
    where
        allOrientationsTiles' = allOrientationsTiles tiles
        borderIdToTileNums' = borderIdToTileNums allOrientationsTiles'
        edgeBorderIds' = edgeBorderIds borderIdToTileNums'
        tileNumToEdgeCount' = tileNumToEdgeCount tiles edgeBorderIds'
        cornerTiles' = cornerTiles tileNumToEdgeCount'
        botRightTile = head $ L.filter isBotRight allOrientationsTiles'
            where
            isBotRight tile = b `elem` edgeBorderIds' && r `elem` edgeBorderIds'
                where
                    b = (tileBorderIds tile) !! 2
                    r = (tileBorderIds tile) !! 1
        tileGrid = solveGrid allOrientationsTiles' edgeBorderIds' botRightTile
        stitchedGrid' = stitchedGrid tileGrid
        searchGrids = allOrientationsRows stitchedGrid'
        monsterCounts = map countMonsters searchGrids
        monsterCount = maximum monsterCounts
        monsterHashCount = monsterCount * (hashCount monsterText)
        gridHashCount = hashCount $ stitchedGrid'
        result = gridHashCount - monsterHashCount

main :: IO ()
main = do
    let fn = "../input/day20.txt"
    inputText <- readFile fn
    let input = E.fromRight [] $ PCPS.parse inputFile fn inputText
    putStrLn $ show $ answer $ input
