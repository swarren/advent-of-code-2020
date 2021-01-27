import qualified Data.Maybe as Maybe
import qualified Data.Map.Strict as Map
import qualified Data.Text as T
import qualified Data.Text.IO as TIO
import qualified Debug.Trace

parse :: T.Text -> [Int]
parse content = map (read . T.unpack) $ T.splitOn (T.pack ",") content

answer :: [Int] -> Int
-- FIXME: Nothing is technically potentially wrong
answer input = iter initWhenSpoken initLastSpoken Maybe.Nothing initIters
    where
        initWhenSpoken = Map.fromList $ zip input [1..]
        initLastSpoken = last input
        initIters = 1 + length input
        iter :: (Map.Map Int Int) -> Int -> Maybe Int -> Int -> Int
        iter whenSpoken lastSpoken whenLastSpoken 2021 = lastSpoken
        iter whenSpoken lastSpoken whenLastSpoken iters =
            iter nextWhenSpoken thisSpoken thisWhenLastSpoken (iters + 1)
            where
                thisSpoken =
                    if Maybe.isNothing whenLastSpoken then
                        0
                    else
                        iters - (Maybe.fromJust whenLastSpoken) - 1
                thisWhenLastSpoken = Map.lookup thisSpoken whenSpoken
                nextWhenSpoken = Map.insert thisSpoken iters whenSpoken

main :: IO ()
main = do
    content <- TIO.readFile "../input/day15.txt"
    let input = parse content
    putStrLn $ show $ answer input
