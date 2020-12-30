import qualified Data.Set as Set
import qualified Data.Text as T
import qualified Data.Text.IO as TIO

parseLine :: T.Text -> (String, Int)
parseLine line = (ins, param)
    where
        (insT:paramT:_) = T.words line
        ins = T.unpack insT
        param = read $ T.unpack $ T.replace (T.pack "+") T.empty paramT

parse :: T.Text -> [(String, Int)]
parse content = map parseLine $ T.lines $ content

doInstr :: (String, Int) -> Int -> Int -> (Int, Int)
doInstr ("acc", param) pc acc = (pc + 1, acc + param)
doInstr ("jmp", param) pc acc = (pc + param, acc)
doInstr ("nop", param) pc acc = (pc + 1, acc)

corruptedInstruction :: (String, Int) -> (String, Int)
corruptedInstruction ("jmp", param) = ("nop", param)
corruptedInstruction ("nop", param) = ("jmp", param)
corruptedInstruction (opcode, param) = (opcode, param)

corruptedProgram :: Int -> [(String, Int)] -> [(String, Int)]
corruptedProgram 0 (instruction:instructions) = (corruptedInstruction instruction):instructions
corruptedProgram i (instruction:instructions) = instruction:(corruptedProgram (i - 1) instructions)
corruptedProgram _ []                         = []

corruptedPrograms :: [(String, Int)] -> [[(String, Int)]]
corruptedPrograms instructions =
    instructions:[corruptedProgram i instructions | i <- [0..(length instructions)]]

executeOnce :: ([(String, Int)], (Bool, Int, Int)) -> ([(String, Int)], (Bool, Int, Int))
executeOnce (instructions, (complete, pc, acc)) =
    (instructions, (newComplete, newPC, newAcc))
    where
        newComplete = pc >= (length instructions)
        (newPC, newAcc) =
            if complete || newComplete then
                (pc, acc)
            else
                doInstr (instructions !! pc) pc acc

answer' :: [([(String, Int)], (Bool, Int, Int))] -> Int
answer' programState = acc
    where
        newState = map executeOnce programState
        completeStates = filter (\(is, (complete, pc, acc)) -> complete) newState
        acc =
            if null completeStates then
                answer' newState
            else
                (\(is, (complete, pc, acc)) -> acc) $ head completeStates

initState :: (Bool, Int, Int)
initState = (False, 0, 0)

answer :: [(String, Int)] -> Int
answer content =
    answer' $ map (\is -> (is, initState)) $ corruptedPrograms content

main :: IO ()
main = do
    content <- TIO.readFile "day8.txt"
    let input = parse content
    putStrLn $ show $ answer input
