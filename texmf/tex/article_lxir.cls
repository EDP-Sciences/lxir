%%
%% This is file `article_lxir.cls',
%% generated with the docstrip utility.
%%
%% The original source files were:
%%
%% classes_lxir.dtx  (with options: `article')
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
  \renewenvironment{abstract}{%
    \xBegin{abstract}%
      \xBegin{abstractMark}%
      \abstractname
      \xEnd{abstractMark}%
}
      {\xEnd{abstract}}
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
  \setcounter{section}{0}%
  \setcounter{subsection}{0}%
  \gdef\thesection{\@Alph\c@section}}
\def\@part[#1]#2{%
  \xBegin{section}\xBegin{sectionHeader}%
  \xEmptyAA{params}{sectype=part}{level=0}
    \ifnum \c@secnumdepth >\m@ne
      \refstepcounter{part}%
      \addcontentsline{toc}{part}{\thepart\hspace{1em}#1}%
    \else
      \addcontentsline{toc}{part}{#1}%
    \fi
    {\parindent \z@ \raggedright
     \interlinepenalty \@M
     \normalfont
     \ifnum \c@secnumdepth >\m@ne
       \xBegin{sectionMark}\partname\nobreakspace\thepart\xEnd{sectionMark}%
     \fi
     \xBegin{sectionTitle}#2\xEnd{sectionTitle}%
     \markboth{}{}\par}%
    \nobreak
    \vskip 3ex
    \@afterheading%
    \xEnd{sectionHeader}\xEnd{section}%
}
\renewcommand\tableofcontents{%
\xBegin{tableofcontents}
    \section*{\contentsname
        \@mkboth{%
           \MakeUppercase\contentsname}{\MakeUppercase\contentsname}}%
    \@starttoc{toc}%
\xEnd{tableofcontents}
    }
\renewcommand\listoffigures{%
\xBegin{listoffigures}
    \section*{\listfigurename}%
      \@mkboth{\MakeUppercase\listfigurename}%
              {\MakeUppercase\listfigurename}%
    \@starttoc{lof}%
\xEnd{listoffigures}
    }
\renewcommand\listoftables{%
\xBegin{listoftables}
    \section*{\listtablename}%
      \@mkboth{%
          \MakeUppercase\listtablename}%
         {\MakeUppercase\listtablename}%
    \@starttoc{lot}%
 \xEnd{listoftables}
    }
\renewenvironment{thebibliography}[1]
     {\xBegin{thebibliography}\xBegin{thebibliographyMark}\section*{\refname}\xEnd{thebibliographyMark}%
      \@mkboth{\MakeUppercase\refname}{\MakeUppercase\refname}%
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
                \twocolumn[\section*{\indexname}]%
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
%% End of file `article_lxir.cls'.
