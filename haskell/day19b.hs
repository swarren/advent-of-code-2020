import qualified Data.Either as E
import qualified Data.Map as Map
import qualified Data.Maybe as Maybe
import qualified Text.ParserCombinators.Parsec as PCPS

data RuleItem =
    SubRule Int |
    LiteralString String
    deriving (Show)

eol = PCPS.char '\n'

inputRuleItemSubRuleNum = do
    val <- PCPS.many1 PCPS.digit
    return $ SubRule (read val :: Int)

inputRuleItemString = do
    PCPS.char '"'
    s <- PCPS.many1 (PCPS.noneOf "\"")
    PCPS.char '"'
    return $ LiteralString s

inputRuleItem = do
    PCPS.char ' '
    item <- inputRuleItemSubRuleNum PCPS.<|> inputRuleItemString
    return item

inputRule = do
    ruleNum <- PCPS.many1 PCPS.digit
    PCPS.char ':'
    options <- PCPS.sepBy (PCPS.many1 $ PCPS.try inputRuleItem) (PCPS.string " |")
    eol
    return (read ruleNum :: Int, options)

inputMessage = do
    s <- PCPS.many (PCPS.noneOf "\n")
    eol
    return s

inputFile = do
    rules <- PCPS.many inputRule
    eol
    messages <- PCPS.many inputMessage
    PCPS.eof
    return (rules, messages)

matchRuleList :: Map.Map Int [[RuleItem]] -> String -> Int -> [RuleItem] -> [Int]
matchRuleList rules s i [] = [i]
matchRuleList rules s i ruleList =
    foldl (++) [] [matchRuleList rules s si (tail ruleList) | si <- matchRule rules s i (ruleNum $ head ruleList)]
    where
        ruleNum (SubRule n) = n

matchRule :: Map.Map Int [[RuleItem]] -> String -> Int -> Int -> [Int]
matchRule rules s i ruleNum =
    foldl (++) [] [subMatches ruleList | ruleList <- Maybe.fromJust $ Map.lookup ruleNum rules]
    where
        subMatches [(LiteralString s')] =
            if i < (length s) && s !! i == s' !! 0 then [i + 1] else []
        subMatches ruleList =
            matchRuleList rules s i ruleList

isMatch :: Map.Map Int [[RuleItem]] -> String -> Bool
isMatch rules s =
    (length s) `elem` (matchRule rules s 0 0)

answer :: ([(Int, [[RuleItem]])], [String]) -> Int
answer (rulesList, messages) = countMatch
    where
        rulesOrig = Map.fromList rulesList
        modifiedRules = Map.fromList [
            (8,  [[SubRule 42], [SubRule 42, SubRule 8]]),
            (11, [[SubRule 42, SubRule 31], [SubRule 42, SubRule 11, SubRule 31]])]
        rules = Map.union modifiedRules rulesOrig
        isMatches = map (isMatch rules) messages
        countMatch = length $ filter (== True) $ isMatches

main :: IO ()
main = do
    let fn = "../input/day19.txt"
    inputText <- readFile fn
    let input = E.fromRight ([], []) $ PCPS.parse inputFile fn inputText
    putStrLn $ show $ answer $ input
