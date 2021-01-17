import Control.Monad.ST
import Control.Monad
import Data.STRef
import qualified Data.Text as T
import qualified Data.Text.IO as TIO
import qualified Data.Vector.Unboxed.Mutable as V

parse :: T.Text -> [Int]
parse content = map (read . T.unpack) $ T.splitOn (T.pack ",") content

numSpoken = 30000000 :: Int

answer :: [Int] -> Int
answer input = runST $ do
    let l = (maximum $ numSpoken:input) + 1
    whenSpoken <- V.replicate l (-1 :: Int)
    forM_ (zip input [1..]) $ \(numSpoken, iter) -> do
        V.write whenSpoken numSpoken iter
    nextSpokenR <- newSTRef $ 0
    forM_ [(length input) + 1..numSpoken - 1] $ \iter -> do
        thisSpoken <- readSTRef nextSpokenR
        thisSpokenWhen <- V.read whenSpoken thisSpoken
        let nextSpoken =
                if thisSpokenWhen == -1 then
                    0
                else do
                    iter - thisSpokenWhen
        writeSTRef nextSpokenR nextSpoken
        V.write whenSpoken thisSpoken iter
    readSTRef nextSpokenR

main :: IO ()
main = do
    content <- TIO.readFile "day15.txt"
    let input = parse content
    putStrLn $ show $ answer input
