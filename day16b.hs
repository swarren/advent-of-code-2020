import qualified Data.Either as E
import qualified Data.List as L
import qualified Data.Map as M
import qualified Data.Maybe as Maybe
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

validTickets :: [(Int, Int)] -> [[Int]] -> [[Int]]
validTickets fieldRanges tickets =
    filter (\ticket -> null $ invalidFieldVals fieldRanges ticket) tickets

assignFields :: [[Int]] -> M.Map Int Int
assignFields possibleFieldNums =
    assignFields' (zip [0..(length possibleFieldNums)-1] possibleFieldNums) M.empty
    where
        assignFields' :: [(Int, [Int])] -> M.Map Int Int -> M.Map Int Int
        assignFields' possibilities assignments =
            if length possibilities == length assignments then
                assignments
            else
                assignFields' possibilities' assignments'
            where
                assignThisTime = filter (\(fn1, fn2s) -> (length fn2s) == 1) possibilities
                (fn1, fn2:xs) = head assignThisTime
                possibilities' :: [(Int, [Int])]
                possibilities' = map (\(n, ps) -> (n, filter (\v -> v /= fn2) ps)) possibilities
                assignments' :: M.Map Int Int
                assignments' = M.insert fn2 fn1 assignments

answer :: ([(String, [(Int, Int)])], [Int], [[Int]]) -> Int
answer (fieldDefs, yourTicket, nearbyTickets) =
    result
    where
        fieldNums :: [Int]
        fieldNums = [0..(length fieldDefs)-1]
        fieldRangesOfFieldDef :: (String, [(Int, Int)]) -> [(Int, Int)]
        fieldRangesOfFieldDef (fieldName, fieldRanges) = fieldRanges
        fieldRangesByFieldDefNum :: [[(Int, Int)]]
        fieldRangesByFieldDefNum = map fieldRangesOfFieldDef fieldDefs
        allFieldRanges :: [(Int, Int)]
        allFieldRanges = concat fieldRangesByFieldDefNum
        validNearbyTickets :: [[Int]]
        validNearbyTickets = validTickets allFieldRanges nearbyTickets
        valsOfTicketFieldNum :: Int -> [Int]
        valsOfTicketFieldNum fieldNum = map (\ticket -> ticket !! fieldNum) validNearbyTickets
        valsByTicketFieldNum :: [[Int]]
        valsByTicketFieldNum = map valsOfTicketFieldNum fieldNums
        isValidFieldRangesForValue :: [(Int, Int)] -> Int -> Bool
        isValidFieldRangesForValue fieldRanges value = any (\fr -> validFieldRangeVal fr value) fieldRanges
        isValidFieldRangesForValues :: [(Int, Int)] -> [Int] -> Bool
        isValidFieldRangesForValues fieldRanges values = all (isValidFieldRangesForValue fieldRanges) values
        isValidFieldDefNumForValues :: Int -> [Int] -> Bool
        isValidFieldDefNumForValues fieldDefNum values = isValidFieldRangesForValues (fieldRangesByFieldDefNum !! fieldDefNum) values
        allPossibleFieldDefNumsForValues :: [Int] -> [Int]
        allPossibleFieldDefNumsForValues vals = filter (\n -> isValidFieldDefNumForValues n vals) fieldNums
        allPossibleFieldDefNumsForTicketFieldNum :: Int -> [Int]
        allPossibleFieldDefNumsForTicketFieldNum ticketFieldNum = allPossibleFieldDefNumsForValues $ valsByTicketFieldNum !! ticketFieldNum
        possibleFieldDefNumsByTicketFieldNum :: [[Int]]
        possibleFieldDefNumsByTicketFieldNum = map allPossibleFieldDefNumsForTicketFieldNum fieldNums
        fieldDefNumToTicketFieldNum :: M.Map Int Int
        fieldDefNumToTicketFieldNum = assignFields possibleFieldDefNumsByTicketFieldNum
        departureFieldDefNums :: [Int]
        departureFieldDefNums = map (\(fn, fd) -> fn) $ filter (\(fnum, (fname, frs)) -> L.isPrefixOf "departure" fname) $ zip fieldNums fieldDefs
        departureTicketFieldNums :: [Int]
        departureTicketFieldNums = map (\fdn -> Maybe.fromJust $ M.lookup fdn fieldDefNumToTicketFieldNum) departureFieldDefNums
        yourTicketDepartureFieldVals :: [Int]
        yourTicketDepartureFieldVals = map (\fn -> yourTicket !! fn) departureTicketFieldNums
        result :: Int
        result = product yourTicketDepartureFieldVals

main :: IO ()
main = do
    let fn = "day16.txt"
    inputText <- readFile fn
    putStrLn $ show $ answer $ input fn inputText
    where
        input :: String -> String -> ([(String, [(Int, Int)])], [Int], [[Int]])
        input fn inputText = E.fromRight ([], [], []) $ PCPS.parse inputFile fn inputText
