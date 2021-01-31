import Data.Bits
import qualified Data.List as List
import qualified Data.Text as T
import qualified Data.Text.IO as TIO

parse :: T.Text -> [T.Text]
parse content = T.lines content

seatCharMap :: Char -> Char
seatCharMap 'B' = '1'
seatCharMap 'R' = '1'
seatCharMap _   = '0'

bin2dec :: String -> Int
bin2dec s = List.foldl' (\v c -> v * 2 + c) 0 $ map c2i s
    where
        c2i c = if c == '1' then 1 else 0

seatNumber :: T.Text -> Int
seatNumber s = bin2dec $ T.unpack $ T.map seatCharMap s

answer :: [T.Text] -> Int
answer content = List.maximum $ map seatNumber $ content

main :: IO ()
main = do
    content <- TIO.readFile "../input/day5.txt"
    let input = parse content
    putStrLn $ show $ answer input
