import qualified Data.Either as E
import qualified Data.List as L
import qualified Text.ParserCombinators.Parsec as PCPS

eol = PCPS.char '\n'

-- 251-961
inputFieldRange = do
    start <- PCPS.many1 PCPS.digit
    PCPS.char '-'
    end <- PCPS.many1 PCPS.digit
    return (read start :: Int, read end :: Int)

-- departure location: 43-237 or 251-961
inputField = do
    fieldName <- PCPS.many1 (PCPS.noneOf ":\n")
    PCPS.string ": "
    fieldRanges <- PCPS.sepBy inputFieldRange (PCPS.string " or ")
    eol
    return (fieldName, fieldRanges)

inputFields = PCPS.many1 inputField

commaSeparatedIntLine = do
    vals <- PCPS.sepBy (PCPS.many1 PCPS.digit) (PCPS.char ',')
    eol
    return (map read vals :: [Int])

inputYourTicket = do
    PCPS.string "your ticket:" >> eol
    commaSeparatedIntLine

inputNearbyTickets = do
    PCPS.string "nearby tickets:" >> eol
    PCPS.many1 commaSeparatedIntLine

inputFile = do
    fields <- inputFields
    eol
    yourTicket <- inputYourTicket
    eol
    nearbyTickets <- inputNearbyTickets
    return (fields, yourTicket, nearbyTickets)

validFieldRangeVal :: (Int, Int) -> Int -> Bool
validFieldRangeVal (rMin, rMax) v =
    rMin <= v && v <= rMax

invalidFieldRangeVal :: (Int, Int) -> Int -> Bool
invalidFieldRangeVal fieldRange v =
    not $ validFieldRangeVal fieldRange v

validFieldVal :: [(Int, Int)] -> Int -> Bool
validFieldVal fieldRanges fieldVal =
    any (\fd -> validFieldRangeVal fd fieldVal) fieldRanges 

invalidFieldVal :: [(Int, Int)] -> Int -> Bool
invalidFieldVal fieldRanges fieldVal =
    not $ validFieldVal fieldRanges fieldVal

invalidFieldVals :: [(Int, Int)] -> [Int] -> [Int]
invalidFieldVals fieldRanges ticket =
    filter (invalidFieldVal fieldRanges) ticket

answer :: ([(String, [(Int, Int)])], [Int], [[Int]]) -> Int
answer (fieldDefs, yourTicket, nearbyTickets) =
    sum $ map (\ticket -> sum $ invalidFieldVals allFieldRanges ticket) nearbyTickets
    where
        allFieldRanges = concat $ map (\(fd, fr) -> fr) fieldDefs

main :: IO ()
main = do
    let fn = "day16.txt"
    inputText <- readFile fn
    putStrLn $ show $ answer $ input fn inputText
    where
        input :: String -> String -> ([(String, [(Int, Int)])], [Int], [[Int]])
        input fn inputText = E.fromRight ([], [], []) $ PCPS.parse inputFile fn inputText
