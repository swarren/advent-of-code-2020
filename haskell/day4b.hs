import qualified Data.List as List
import qualified Data.Map as Map
import qualified Data.Maybe as Maybe
import qualified Data.Set as Set
import qualified Data.Text as T
import qualified Data.Text.IO as TIO
import Text.Regex.PCRE

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

validField' :: String -> T.Text -> Bool

validField' "byr" v =
    (v' >= 1920) && (v' <= 2002)
    where
        v' = read $ T.unpack $ v

validField' "iyr" v =
    (v' >= 2010) && (v' <= 2020)
    where
        v' = read $ T.unpack $ v

validField' "eyr" v =
    (v' >= 2020) && (v' <= 2030)
    where
        v' = read $ T.unpack $ v

validField' "hgt" v =
    valid' units val
    where
        val = read $ T.unpack $ T.dropEnd 2 v
        units = T.unpack $ T.takeEnd 2 v
        valid' "cm" val = (val >= 150) && (val <= 193)
        valid' "in" val = (val >= 59) && (val <= 76)
        valid' _    _   = False

validField' "hcl" v = (T.unpack v) =~ "^#[0-9a-f]{6}$"

validField' "ecl" v = Set.member (T.unpack v) $ Set.fromList ["amb", "blu", "brn", "gry", "grn", "hzl", "oth"]

validField' "pid" v = (T.unpack v) =~ "^[0-9]{9}$"

validField' _ v = False

validField :: (T.Text, T.Text) -> Bool
validField (f, v) =
    validField' (T.unpack f) v

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
    validKeys && validFields
    where
        p' = (Map.delete $ T.pack "cid") $ p
        validKeys = (Map.keys p') == wantedKeys
        validFields = List.and $ map validField $ Map.assocs $ p'

answer :: [Map.Map T.Text T.Text] -> Int
answer content = length $ filter validPassport $ content

main :: IO ()
main = do
    content <- TIO.readFile "../input/day4.txt"
    let input = parse content
    putStrLn $ show $ answer input
