import qualified Data.Text

data Entry = Entry {
    pos1 :: Int,
    pos2 :: Int,
    c :: Char,
    password :: String} deriving (Show)

-- 9-12 q: qqqxhnhdmqqqqjz
parseline line =
    Entry pos1 pos2 c password
    where
        fields =
            words $
            Data.Text.unpack $
            Data.Text.replace (Data.Text.pack "-") (Data.Text.pack " ") $
            Data.Text.replace (Data.Text.pack ":") (Data.Text.pack "") $
            Data.Text.pack $
            line
        pos1     = read $ fields !! 0
        pos2     = read $ fields !! 1
        c        = head $ fields !! 2
        password = fields !! 3

parse content =
    map parseline $
    lines $
    content

entry_match entry =
    match1 /= match2
    where
        c1 = head $ drop ((pos1 entry) - 1) $ password entry
        match1 = c1 == c entry
        c2 = head $ drop ((pos2 entry) - 1) $ password entry
        match2 = c2 == c entry

answer input =
    length $ filter entry_match input

main = do
    content <- readFile "day2.txt"
    let input = parse content
    putStrLn $ show $ answer input

