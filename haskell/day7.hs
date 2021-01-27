import qualified Data.List as List
import qualified Data.Map as Map
import qualified Data.Maybe as Maybe
import qualified Data.Text as T
import qualified Data.Text.IO as TIO

parseCountBag :: T.Text -> (Int, T.Text)
parseCountBag cb = (c, b)
    where
        w = T.words cb
        c = read $ T.unpack $ head w
        b = T.unwords $ take ((length w) - 2) $ tail w

-- dotted blue bags contain 3 wavy bronze bags, 5 clear tomato bags.
parseLine :: T.Text -> (T.Text, [(Int, T.Text)])
parseLine line = (thisBag, subBags)
    where
        (thisBag:subBagsT:_) = T.splitOn (T.pack " bags contain ") $ T.dropEnd 1 line
        subBagsL =
            if subBagsT == (T.pack "no other bags") then
                []
            else
                T.splitOn (T.pack ", ") subBagsT
        subBags = map parseCountBag subBagsL

parse :: T.Text -> Map.Map T.Text [(Int, T.Text)]
parse content = Map.fromList $ map parseLine $ T.lines content

isSubBagGold :: (Int, T.Text) -> Bool
isSubBagGold (c, b) = (b == (T.pack "shiny gold"))

canContainGold :: Map.Map T.Text [(Int, T.Text)] -> T.Text -> Bool
canContainGold m b = directContains || recursiveContains
    where
        subBags = Maybe.fromJust $ Map.lookup b m
        directContains = any isSubBagGold subBags
        recursiveContains = any (canContainGold m) $ map snd subBags

answer :: Map.Map T.Text [(Int, T.Text)] -> Int
answer content = length $ filter (\b -> b) $ map (canContainGold content) $ Map.keys content

main :: IO ()
main = do
    content <- TIO.readFile "../input/day7.txt"
    let input = parse content
    putStrLn $ show $ answer input
