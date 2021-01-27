import Data.Bits
import qualified Data.Either as E
import qualified Data.List as L
import qualified Data.Map.Strict as Map
import qualified Text.ParserCombinators.Parsec as PCPS

data State = State {
    stateMaskZeroes :: Int,
    stateMaskOnes :: Int,
    stateMaskXs :: Int,
    memory :: Map.Map Int Int
} deriving (Show)

data Operation =
    SetMask {
        maskZeroes :: Int,
        maskOnes :: Int,
        maskXs :: Int
    } |
    WriteMem {
        addr :: Int,
        val :: Int
    }
    deriving (Show)
    
inputFile = PCPS.endBy inputLine (PCPS.char '\n')

inputLine = PCPS.try maskLine PCPS.<|> memWriteLine

maskLine = PCPS.string "mask = " >> maskString

bin2int :: String -> Int
bin2int s = L.foldl' (\v c -> v * 2 + c) 0 $ map c2i s
    where
        c2i c = if c == '1' then 1 else 0

maskStrToInt c s = bin2int $ map (\x -> if x == c then '1' else '0') s

maskString = do
    mask <- PCPS.many1 $ PCPS.oneOf "01X"
    return $ SetMask (maskStrToInt '0' mask) (maskStrToInt '1' mask) (maskStrToInt 'X' mask)

memWriteLine = do
    PCPS.string "mem["
    addr <- PCPS.many1 PCPS.digit
    PCPS.string "] = "
    val <- PCPS.many1 PCPS.digit
    return $ WriteMem (read addr) (read val)

applyOp :: State -> Operation -> State

applyOp (State s0 s1 sx m) (SetMask sm0 sm1 smz) = newState
    where
        newState = State sm0 sm1 smz m

applyOp (State s0 s1 sx m) (WriteMem a v) = newState
    where
        newState = State s0 s1 sx newM
        writeVal = (v .|. s1) .&. (complement s0)
        newM = Map.insert a writeVal m

answer :: [Operation] -> Int
answer ops = memSum $ memory finalState
    where
        initState = State 0 0 0 Map.empty
        finalState = L.foldl' applyOp initState ops
        memSum m = Map.foldr (+) 0 m

main :: IO ()
main = do
    let fn = "../input/day14.txt"
    inputText <- readFile fn
    let input = E.fromRight [] $ PCPS.parse inputFile fn inputText
    putStrLn $ show $ answer $ input
