%%
%% This is file `book_lxir.cls',
%% generated with the docstrip utility.
%%
%% The original source files were:
%%
%% classes_lxir.dtx  (with options: `book')
%% 
%% This is a generated file.
%% 
%% Copyright EDP Sciences 2007
%% 
%% This file was generated from file(s) of the lxir  system.
%% --------------------------------------------------------------
%% 
%% \CharacterTable
%%  {Upper-case    \A\B\C\D\E\F\G\H\I\J\K\L\M\N\O\P\Q\R\S\T\U\V\W\X\Y\Z
%%   Lower-case    \a\b\c\d\e\f\g\h\i\j\k\l\m\n\o\p\q\r\s\t\u\v\w\x\y\z
%%   Digits        \0\1\2\3\4\5\6\7\8\9
%%   Exclamation   \!     Double quote  \"     Hash (number) \#
%%   Dollar        \$     Percent       \%     Ampersand     \&
%%   Acute accent  \'     Left paren    \(     Right paren   \)
%%   Asterisk      \*     Plus          \+     Comma         \,
%%   Minus         \-     Point         \.     Solidus       \/
%%   Colon         \:     Semicolon     \;     Less than     \<
%%   Equals        \=     Greater than  \>     Question mark \?
%%   Commercial at \@     Left bracket  \[     Backslash     \\
%%   Right bracket \]     Circumflex    \^     Underscore    \_
%%   Grave accent  \`     Left brace    \{     Vertical bar  \|
%%   Right brace   \}     Tilde         \~}
\NeedsTeXFormat{LaTeX2e}[1995/12/01]
\setlength\textwidth{.99\maxdimen}
\renewcommand\maketitle{%
  \xBegin{maketitle}%
  \begingroup
    \renewcommand\thefootnote{\@fnsymbol\c@footnote}%
    \def\@makefnmark{\rlap{\@textsuperscript{\normalfont\@thefnmark}}}%
    \long\def\@makefntext##1{\parindent 1em\noindent
            \hb@xt@1.8em{%
                \hss\@textsuperscript{\normalfont\@thefnmark}}##1}%
            \@maketitle\@thanks
  \endgroup
  \xEnd{maketitle}%
  \setcounter{footnote}{0}%
  \global\let\thanks\relax
  \global\let\maketitle\relax
  \global\let\@maketitle\relax
  \global\let\@thanks\@empty
  \global\let\@author\@empty
  \global\let\@date\@empty
  \global\let\@title\@empty
  \global\let\title\relax
  \global\let\author\relax
  \global\let\date\relax
  \global\let\and\relax
}
\def\@maketitle{%
\@title%
\@author%
\@date%
}
\renewenvironment{description}
               {\xBegin{list}\xEmptyA{params}{listType=description}\list{}{\labelwidth\z@ \itemindent-\leftmargin
                        \let\makelabel\descriptionlabel}}
               {\endlist\endlist\xEnd{list}}
\renewenvironment{verse}
               {\xBegin{verse}% [LXir] no more  \let\\\@centercr
                \list{}{\itemsep      \z@
                        \itemindent   -1.5em%
                        \listparindent\itemindent
                        \rightmargin  \leftmargin
                        \advance\leftmargin 1.5em}%
                \item\relax}
               {\endlist\xEnd{verse}}
\renewenvironment{quotation}
               {\xBegin{quotation}\list{}{\listparindent 1.5em%
                        \itemindent    \listparindent
                        \rightmargin   \leftmargin
                        \parsep        \z@ \@plus\p@}%
                \item\relax}
               {\endlist\xEnd{quotation}}
\renewenvironment{quote}
               {\xBegin{quote}\list{}{\rightmargin\leftmargin}%
                \item\relax}
               {\endlist\xEnd{quote}}
\if@compatibility
\newenvironment{titlepage}
    {%
      \xBegin{titlePage}
      \if@twocolumn
        \@restonecoltrue\onecolumn
      \else
        \@restonecolfalse\newpage
      \fi
      \thispagestyle{empty}%
      \setcounter{page}\z@
    }%
    {\xEnd{titlePage}\if@restonecol\twocolumn \else \newpage \fi

    }
\else
\renewenvironment{titlepage}
    {%
      \xBegin{titlePage}
      \if@twocolumn
        \@restonecoltrue\onecolumn
      \else
        \@restonecolfalse\newpage
      \fi
      \thispagestyle{empty}%
      \setcounter{page}\@ne
    }%
    {\xEnd{titlePage}\if@restonecol\twocolumn \else \newpage \fi
     \if@twoside\else
        \setcounter{page}\@ne
     \fi
    }
\fi
\renewcommand\appendix{\xEmpty{appendix}\par
  \setcounter{chapter}{0}%
  \setcounter{section}{0}%
  \gdef\@chapapp{\appendixname}%
  \gdef\thechapter{\@Alph\c@chapter}}
\def\@part[#1]#2{%
  \xBegin{section}\xBegin{sectionHeader}%
  \xEmptyAA{params}{sectype=part}{level=-1}
    \ifnum \c@secnumdepth >-2\relax
      \refstepcounter{part}%
      \addcontentsline{toc}{part}{\thepart\hspace{1em}#1}%
    \else
      \addcontentsline{toc}{part}{#1}%
    \fi
    \markboth{}{}%
    {\centering
     \interlinepenalty \@M
     \normalfont
     \ifnum \c@secnumdepth >-2\relax
       \xBegin{sectionMark}\partname\nobreakspace\thepart\xEnd{sectionMark}
       \vskip 20\p@
     \fi
     \xBegin{sectionTitle}#2\xEnd{sectionTitle}}%
    \@endpart%
\xEnd{sectionHeader}\xEnd{section}%
}
\def\@makechapterhead#1{%
  \xBegin{section}\xBegin{sectionHeader}\xEmptyAA{params}{sectype=chapter}{level=0}%
  \ifnum \c@secnumdepth >\m@ne
      \if@mainmatter
        \xBegin{sectionMark}\@chapapp\space \thechapter\xEnd{sectionMark}%
      \fi
  \fi
  \xBegin{sectionTitle}#1\xEnd{sectionTitle}%
\xEnd{sectionHeader}\xEnd{section}}
\def\@makeschapterhead#1{%
 \xBegin{section}\xBegin{sectionHeader}\xEmptyAA{params}{sectype=chapter}{level=0}%
\xBegin{sectionTitle}#1\xEnd{sectionTitle}\xEnd{sectionHeader}\xEnd{section}%
}
\renewcommand\tableofcontents{%
\xBegin{tableofcontents}
    \if@twocolumn
      \@restonecoltrue\onecolumn
    \else
      \@restonecolfalse
    \fi
    \chapter*{\contentsname
        \@mkboth{%
           \MakeUppercase\contentsname}{\MakeUppercase\contentsname}}%
    \@starttoc{toc}%
    \if@restonecol\twocolumn\fi
\xEnd{tableofcontents}
    }
\renewcommand\listoffigures{%
\xBegin{listoffigures}
    \if@twocolumn
      \@restonecoltrue\onecolumn
    \else
      \@restonecolfalse
    \fi
    \chapter*{\listfigurename}%
      \@mkboth{\MakeUppercase\listfigurename}%
              {\MakeUppercase\listfigurename}%
    \@starttoc{lof}%
    \if@restonecol\twocolumn\fi
\xEnd{listoffigures}
    }
\renewcommand\listoftables{%
\xBegin{listoftables}
    \if@twocolumn
      \@restonecoltrue\onecolumn
    \else
      \@restonecolfalse
    \fi
    \chapter*{\listtablename}%
      \@mkboth{%
          \MakeUppercase\listtablename}%
         {\MakeUppercase\listtablename}%
    \@starttoc{lot}%
    \if@restonecol\twocolumn\fi
 \xEnd{listoftables}
    }
\renewenvironment{thebibliography}[1]
     {\xBegin{thebibliopraphy}\xBegin{thebibliographyMark}\chapter*{\bibname}\xEnd{thebibliographyMark}%
      \@mkboth{\MakeUppercase\bibname}{\MakeUppercase\bibname}%
      \list{\@biblabel{\@arabic\c@enumiv}}%
           {\settowidth\labelwidth{\@biblabel{#1}}%
            \leftmargin\labelwidth
            \advance\leftmargin\labelsep
            \@openbib@code
            \usecounter{enumiv}%
            \let\p@enumiv\@empty
            \renewcommand\theenumiv{\@arabic\c@enumiv}}%
      \sloppy
      \clubpenalty4000
      \@clubpenalty \clubpenalty
      \widowpenalty4000%
      \sfcode`\.\@m}
     {\def\@noitemerr
       {\@latex@warning{Empty `thebibliography' environment}}%
      \endlist\xEnd{thebibliography}}
\renewenvironment{theindex}
               {\xBegin{theindex}\if@twocolumn
                  \@restonecolfalse
                \else
                  \@restonecoltrue
                \fi
                \twocolumn[\@makeschapterhead{\indexname}]%
                \@mkboth{\MakeUppercase\indexname}%
                        {\MakeUppercase\indexname}%
                \thispagestyle{plain}\parindent\z@
    \begin{macrocode}
                \parskip\z@ \@plus .3\p@\relax
                \columnseprule \z@
                \columnsep 35\p@
                \let\item\@idxitem}
               {\if@restonecol\onecolumn\else\clearpage\fi\xEnd{theindex}}
\endinput
%%
%% End of file `book_lxir.cls'.
