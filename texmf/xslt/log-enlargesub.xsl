<?xml version="1.0" encoding="UTF-8"?>
<!--
%% log-enlargesub.xsl
%% part of lxir - a tool to convert latex documents to xml.
%% Copyright 2010 EDP-Sciences
%
% This work may be distributed and/or modified under the
% conditions of the LaTeX Project Public License, either version 1.3
% of this license or (at your option) any later version.
% The latest version of this license is in
%   http://www.latex-project.org/lppl.txt
% and version 1.3 or later is part of all distributions of LaTeX
% version 2005/12/01 or later.
%
% This work has the LPPL maintenance status `maintained'.
%
% The Current Maintainer of this work is Xavier Trochu.
%
% This work consists of the files log-enlargesub.xsl.
-->
<xsl:stylesheet version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>
<xsl:output method="xml" indent="no" encoding="UTF-8"/>

<!--
	This stylesheet replace the following:

	<mn1 />
	<msub>
		<mn2 />
		<foo />
	</msub>

	into:
	<msub>
		<mn 1 + 2 />
		<foo />
	</msub>
-->

<!-- remove the tag here -->
<xsl:template match="mn[following-sibling::*[position() = 1][self::msub or self::msup][child::*[position() = 1][self::mn]]]" />

<xsl:template match="*[self::msub or self::msup][preceding-sibling::*[position() = 1][self::mn]][child::*[position() = 1][self::mn]]">
	<xsl:copy>
		<xsl:apply-templates select="@*" />
		<mn>
			<!-- copy the attributes from the first tag -->
			<xsl:apply-templates select="preceding-sibling::mn[position() = 1]/@*" />
			<!-- concatenate the value of both tags -->
			<xsl:value-of select="preceding-sibling::mn[position() = 1]/text()" />
			<xsl:value-of select="mn[position() = 1]/text()" />
		</mn>
		<xsl:apply-templates select="*[position() != 1]" />
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
