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

execute :: [(String, Int)] -> Set.Set Int -> Int -> Int -> Int
execute instructions seenPCs pc acc =
    if Set.member pc seenPCs then
        acc
    else
        execute instructions newSeenPCs newPC newAcc
    where
        newSeenPCs = Set.insert pc seenPCs
        (newPC, newAcc) = doInstr (instructions !! pc) pc acc

answer :: [(String, Int)] -> Int
answer content =
    execute content Set.empty 0 0

main :: IO ()
main = do
    content <- TIO.readFile "../input/day8.txt"
    let input = parse content
    putStrLn $ show $ answer input
