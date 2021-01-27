import qualified Data.Either as E
import qualified Data.Set as S
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

game' :: S.Set ([Int], [Int]) -> [Int] -> [Int] -> (Bool, [Int])
game' prevStates hand1 hand2 =
    if (hand1, hand2) `elem` prevStates then
        (True, hand1)
    else if null hand1 || null hand2 then
        (null hand2, if null hand1 then hand2 else hand1)
    else
        game' prevStates' hand1' hand2'
        where
            card1:remaining1 = hand1
            card2:remaining2 = hand2
            canRecurse1 = card1 <= length remaining1
            canRecurse2 = card2 <= length remaining2
            canRecurse = canRecurse1 && canRecurse2
            winPlayer1 = if canRecurse then recursiveWin1 else card1 >= card2
                where
                    (recursiveWin1, _) = game' S.empty (take card1 remaining1) (take card2 remaining2)
            cards = if winPlayer1 then [card1, card2] else [card2, card1]
            prevStates' = S.insert (hand1, hand2) prevStates
            hand1' = remaining1 ++ if winPlayer1 then cards else []
            hand2' = remaining2 ++ if not winPlayer1 then cards else []

game :: [Int] -> [Int] -> (Bool, [Int])
game hand1 hand2 =
    game' S.empty hand1 hand2

answer :: ([Int], [Int]) -> Int
answer (hand1, hand2) = score winningHand
    where
        (win1, winningHand) = game hand1 hand2

main :: IO ()
main = do
    let fn = "../input/day22.txt"
    inputText <- readFile fn
    let input = E.fromRight undefined $ PCPS.parse inputFile fn inputText
    putStrLn $ show $ answer $ input
