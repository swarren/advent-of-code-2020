import qualified Data.List as List
import qualified Data.Map as Map
import qualified Data.Set as Set
import qualified Data.Text as T
import qualified Data.Text.IO as TIO

tuple :: [a] -> (a, a)
tuple (x:y:[]) = (x, y)

parseLine :: [Map.Map T.Text T.Text] -> T.Text -> [Map.Map T.Text T.Text]
parseLine (r:rs) line
    | line == T.empty = Map.empty:r:rs
    | otherwise       = r':rs
    where
        entries =
            map (T.splitOn $ T.singleton ':') $
            T.words line
        r' = Map.union r $ Map.fromList $ map tuple entries

parse :: T.Text -> [Map.Map T.Text T.Text]
parse content =
    reverse $
    List.foldl' parseLine [Map.empty] $
    T.lines $
    content

wantedKeys :: [T.Text]
wantedKeys =
    map T.pack $ List.sort $ [
        "byr",
        "iyr",
        "eyr",
        "hgt",
        "hcl",
        "ecl",
        "pid"
    ]

validPassport :: Map.Map T.Text T.Text -> Bool
validPassport p =
    validKeys
    where
        p' = (Map.delete $ T.pack "cid") $ p
        validKeys = (Map.keys p') == wantedKeys

answer :: [Map.Map T.Text T.Text] -> Int
answer content =
    length $
    filter validPassport $
    content

main :: IO ()
main = do
    content <- TIO.readFile "../input/day4.txt"
    let input = parse content
    putStrLn $ show $ answer input
