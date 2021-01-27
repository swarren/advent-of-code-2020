import qualified Data.Either as E
import qualified Text.Parsec.Expr as TPE
import qualified Text.Parsec.Token as TPTok
import qualified Text.Parsec.Language as TPLang
import qualified Text.ParserCombinators.Parsec as PCPS

parse :: String -> [String]
parse content = lines content

lexer :: TPTok.TokenParser ()
lexer = TPTok.makeTokenParser style
  where
    style = TPLang.emptyDef
      { TPTok.reservedOpNames = ["+", "*"],
        TPTok.reservedNames   = [],
        TPTok.identStart      = PCPS.letter,
        TPTok.identLetter     = PCPS.alphaNum
      }

binary name fun assoc = TPE.Infix (TPTok.reservedOp lexer name >> return fun) assoc

term = TPTok.parens lexer evalExpr PCPS.<|> TPTok.natural lexer

table = [[binary "+" (+) TPE.AssocLeft], [binary "*" (*) TPE.AssocLeft]]

evalExpr = TPE.buildExpressionParser table term

answer :: [String] -> Integer
answer expressions =
    sum $ map (\expr -> E.fromRight 0 $ PCPS.parse evalExpr "expr" expr) expressions

main :: IO ()
main = do
    content <- readFile "../input/day18.txt"
    let input = parse content
    putStrLn $ show $ answer input
