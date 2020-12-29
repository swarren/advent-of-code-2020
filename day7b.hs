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

countBags :: Map.Map T.Text [(Int, T.Text)] -> T.Text -> Int
countBags m b = 1 + subBagCount
    where
        subBags = Maybe.fromJust $ Map.lookup b m
        sbc (subBagCount, subBag) = subBagCount * (countBags m subBag)
        subBagCounts = map sbc subBags
        subBagCount = List.foldl' (+) 0 subBagCounts

answer :: Map.Map T.Text [(Int, T.Text)] -> Int
answer m = (countBags m (T.pack "shiny gold")) - 1

main :: IO ()
main = do
    content <- TIO.readFile "day7.txt"
    let input = parse content
    putStrLn $ show $ answer input
