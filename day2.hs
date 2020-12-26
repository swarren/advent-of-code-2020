import qualified Data.Text

data Entry = Entry {
    cmin :: Int,
    cmax :: Int,
    c :: Char,
    password :: String} deriving (Show)

-- 9-12 q: qqqxhnhdmqqqqjz
parseline line =
    Entry cmin cmax c password
    where
        fields =
            words $
            Data.Text.unpack $
            Data.Text.replace (Data.Text.pack "-") (Data.Text.pack " ") $
            Data.Text.replace (Data.Text.pack ":") (Data.Text.pack "") $
            Data.Text.pack $
            line
        cmin     = read $ fields !! 0
        cmax     = read $ fields !! 1
        c        = head $ fields !! 2
        password = fields !! 3

parse content =
    map parseline $
    lines $
    content

entry_match entry =
    (count >= (cmin entry)) && (count <= (cmax entry))
    where
        count = length $ filter (\x -> x == (c entry)) (password entry)

answer input =
    length $ filter entry_match input

main = do
    content <- readFile "day2.txt"
    let input = parse content
    putStrLn $ show $ answer input
