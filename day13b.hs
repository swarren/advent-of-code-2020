import qualified Data.Maybe as M
import qualified Data.List as L
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

answer :: (Int, [Maybe Int]) -> Int
answer (time, busses) = answer' 0
    where
        bussesOffset' :: [(Maybe Int, Int)]
        bussesOffset' = zip busses [0..]
        bussesOffset'' :: [(Int, Int)]
        bussesOffset'' = map (\(bus, ofs) -> (M.fromJust bus, ofs)) $ filter (\(bus, ofs) -> M.isJust bus) bussesOffset'
        answer' :: Int -> Int
        answer' n = if isMatch t then t else answer' (n + 1)
            where
                t = (937 * (569 * n + 14)) - 44
                isMatch t = L.and $ map (\(bus, ofs) -> (t + ofs) `mod` bus == 0) bussesOffset''

main :: IO ()
main = do
    content <- TIO.readFile "day13.txt"
    let input = parse content
    putStrLn $ show $ answer input
