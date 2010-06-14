<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>

<xsl:output method="xml" indent="no" encoding="UTF-8"/>

<xsl:template match="msup[@single='1']">
	<xsl:choose>
		<xsl:when test="following-sibling::*[position() = 1][self::msub]">
			<mmultiscripts>
				<xsl:apply-templates select="following-sibling::*[position() = 1]/child::*[position() = 1]" />
				<xsl:apply-templates select="following-sibling::*[position() = 1]/child::*[position() = last()]" />
				<none />
				<mprescripts />
				<none />
				<xsl:apply-templates select="*" />
			</mmultiscripts>
		</xsl:when>
		<xsl:when test="following-sibling::*[position() = 1][self::msup]">
			<mmultiscripts>
				<xsl:apply-templates select="following-sibling::*[position() = 1]/child::*[position() = 1]" />
				<none />
				<xsl:apply-templates select="following-sibling::*[position() = 1]/child::*[position() = last()]" />
				<mprescripts />
				<none />
				<xsl:apply-templates select="*" />
			</mmultiscripts>
		</xsl:when>
		<xsl:when test="following-sibling::*[position() = 1][self::msubsup]">
			<mmultiscripts>
				<xsl:apply-templates select="following-sibling::*[position() = 1]/*" />
				<mprescripts />
				<none />
				<xsl:apply-templates select="*" />
			</mmultiscripts>
		</xsl:when>
		<xsl:otherwise>
			<xsl:copy>
				<xsl:apply-templates select="*|@*|text()"/>
			</xsl:copy>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template match="msub[@single='1']">
	<xsl:choose>
		<xsl:when test="following-sibling::*[position() = 1][self::msub]">
			<mmultiscripts>
				<xsl:apply-templates select="following-sibling::*[position() = 1]/child::*[position() = 1]" />
				<xsl:apply-templates select="following-sibling::*[position() = 1]/child::*[position() = last()]" />
				<none />
				<mprescripts />
				<xsl:apply-templates select="*" />
				<none />
			</mmultiscripts>
		</xsl:when>
		<xsl:when test="following-sibling::*[position() = 1][self::msup]">
			<mmultiscripts>
				<xsl:apply-templates select="following-sibling::*[position() = 1]/child::*[position() = 1]" />
				<none />
				<xsl:apply-templates select="following-sibling::*[position() = 1]/child::*[position() = last()]" />
				<mprescripts />
				<xsl:apply-templates select="*" />
				<none />
			</mmultiscripts>
		</xsl:when>
		<xsl:when test="following-sibling::*[position() = 1][self::msubsup]">
			<mmultiscripts>
				<xsl:apply-templates select="following-sibling::*[position() = 1]/*" />
				<mprescripts />
				<xsl:apply-templates select="*" />
				<none />
			</mmultiscripts>
		</xsl:when>
		<xsl:otherwise>
			<xsl:copy>
				<xsl:apply-templates select="*|@*|text()"/>
			</xsl:copy>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template match="*[self::msup or self::msub or self::msubsup][preceding-sibling::*[position() = 1][self::msup or self::msub][@single='1']]" />

<xsl:template match="*|@*">
	<xsl:copy>
		<xsl:apply-templates select="*|@*|text()"/>
	</xsl:copy>
</xsl:template>

</xsl:stylesheet>
