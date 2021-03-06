import qualified Data.Maybe as M
import qualified Data.Text as T
import qualified Data.Text.IO as TIO

parse :: T.Text -> (Int, [Maybe Int])
parse content = (time, busses)
    where
        lines = T.lines content
        time = read $ T.unpack $ head lines
        bussesS = map T.unpack $ T.splitOn (T.pack ",") $ head $ tail lines
        busnum :: String -> Maybe Int
        busnum "x" = Nothing
        busnum s = Just $ read s
        busses = map busnum bussesS

firstAtOrAfter :: Int -> Int -> (Int, Int)
firstAtOrAfter time busnum = (t, busnum)
    where
        r = time `mod` busnum
        t = if r == 0 then time else time + (busnum - r)

minBusAndTime :: (Int, Int) -> (Int, Int) -> (Int, Int)
minBusAndTime (t1, busnum1) (t2, busnum2) =
    if t1 < t2 then
        (t1, busnum1)
    else
        (t2, busnum2)

answer :: (Int, [Maybe Int]) -> Int
answer (time, busses) = waitingTime * earliestBusnum
    where
        busnums = map M.fromJust $ filter M.isJust busses
        earliests = map (firstAtOrAfter time) busnums
        (earliestTime, earliestBusnum) = foldl1 minBusAndTime earliests
        waitingTime = earliestTime - time

main :: IO ()
main = do
    content <- TIO.readFile "../input/day13.txt"
    let input = parse content
    putStrLn $ show $ answer input
