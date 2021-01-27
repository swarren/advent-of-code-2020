import qualified Data.Text as T
import qualified Data.Text.IO as TIO

-- 9-12 q: qqqxhnhdmqqqqjz
parseLine :: T.Text -> (Int, Int, Char, T.Text)
parseLine line =
    (pos1, pos2, c, password)
    where
        [spos1, spos2, sc, password] =
            T.words $
            T.map (\c -> if c == '-' then ' ' else c) $
            T.filter (\c -> c /= ':') $
            line
        pos1     = read $ T.unpack $ spos1
        pos2     = read $ T.unpack $ spos2
        c        = T.index sc 0

parse :: T.Text -> [(Int, Int, Char, T.Text)]
parse content =
    map parseLine $
    T.lines $
    content

entryMatch :: (Int, Int, Char, T.Text) -> Bool
entryMatch (pos1, pos2, c, password) =
    match1 /= match2
    where
        c1 = T.index password (pos1 - 1)
        match1 = c1 == c
        c2 = T.index password (pos2 - 1)
        match2 = c2 == c

answer :: [(Int, Int, Char, T.Text)] -> Int
answer input =
    length $ filter entryMatch input

main :: IO ()
main = do
    content <- TIO.readFile "../input/day2.txt"
    let input = parse content
    putStrLn $ show $ answer input
