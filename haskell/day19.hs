import Control.Monad
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

parseRuleItem rules (SubRule n) = do
    let options = Maybe.fromJust $ Map.lookup n rules
    let optionParsers = map (\opt -> PCPS.try $ parseRuleOption rules opt) options
    let optionParser = if length optionParsers > 1 then head $ tail $ optionParsers else head $ optionParsers
    x <- PCPS.choice optionParsers
    return ()

parseRuleItem rules (LiteralString s) = do
    s' <- PCPS.string s
    return ()

parseRuleOption rules items = do
    x <- forM items $ \item -> parseRuleItem rules item
    return ()

parseMessage rules = do
    x <- parseRuleItem rules (SubRule 0)
    PCPS.eof
    return ()

answer :: ([(Int, [[RuleItem]])], [String]) -> Int
answer (rulesList, messages) = countMatch
    where
        rules = Map.fromList rulesList
        isMatch = (\m -> E.isRight $ PCPS.parse (parseMessage rules) "message" m)
        isMatches = map isMatch messages
        countMatch = length $ filter (== True) $ isMatches

main :: IO ()
main = do
    let fn = "../input/day19.txt"
    inputText <- readFile fn
    let input = E.fromRight ([], []) $ PCPS.parse inputFile fn inputText
    putStrLn $ show $ answer $ input
