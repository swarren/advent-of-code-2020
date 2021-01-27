import qualified Data.Either as E
import qualified Text.ParserCombinators.Parsec as PCPS

eol = PCPS.char '\n'

inputHeader = do
    PCPS.string "Player " >> PCPS.digit >> PCPS.char ':' >> eol

inputCard = do
    card <- (PCPS.many1 PCPS.digit) <* eol
    return (read card :: Int)

inputCardList =
    PCPS.many1 inputCard

inputFile = do
    inputHeader
    player1 <- inputCardList
    eol
    inputHeader
    player2 <- inputCardList
    PCPS.eof
    return (player1, player2)

score :: [Int] -> Int
score hand =
    sum $ map (\(c, n) -> c * n) $ zip (reverse hand) [1..]

game :: [Int] -> [Int] -> [Int]
game hand1 hand2 =
    if null hand1 || null hand2 then
        if null hand1 then hand2 else hand1
    else
        game hand1' hand2'
        where
            card1:remaining1 = hand1
            card2:remaining2 = hand2
            winPlayer1 = card1 >= card2
            cards = if winPlayer1 then [card1, card2] else [card2, card1]
            hand1' = remaining1 ++ if winPlayer1 then cards else []
            hand2' = remaining2 ++ if not winPlayer1 then cards else []

answer :: ([Int], [Int]) -> Int
answer (hand1, hand2) = score $ game hand1 hand2

main :: IO ()
main = do
    let fn = "../input/day22.txt"
    inputText <- readFile fn
    let input = E.fromRight undefined $ PCPS.parse inputFile fn inputText
    putStrLn $ show $ answer $ input
