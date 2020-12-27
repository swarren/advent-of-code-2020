import qualified Data.Text as T
import qualified Data.Text.IO as TIO

-- 9-12 q: qqqxhnhdmqqqqjz
parseLine :: T.Text -> (Int, Int, Char, T.Text)
parseLine line =
    (cmin, cmax, c, password)
    where
        [scmin, scmax, sc, password] =
            T.words $
            T.map (\c -> if c == '-' then ' ' else c) $
            T.filter (\c -> c /= ':') $
            line
        cmin     = read $ T.unpack $ scmin
        cmax     = read $ T.unpack $ scmax
        c        = T.index sc 0

parse :: T.Text -> [(Int, Int, Char, T.Text)]
parse content =
    map parseLine $
    T.lines $
    content

entryMatch :: (Int, Int, Char, T.Text) -> Bool
entryMatch (cmin, cmax, c, password) =
    (count >= cmin) && (count <= cmax)
    where
        count = T.length $ T.filter (\x -> x == c) $ password

answer :: [(Int, Int, Char, T.Text)] -> Int
answer input =
    length $ filter entryMatch input

main :: IO ()
main = do
    content <- TIO.readFile "day2.txt"
    let input = parse content
    putStrLn $ show $ answer input
