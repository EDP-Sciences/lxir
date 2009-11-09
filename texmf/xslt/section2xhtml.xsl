<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
		xmlns="http://www.w3.org/1999/xhtml" 
		xmlns:lxir="http://www.latex-lxir.org">

  <xsl:template match="section">
    <xsl:variable name="idSection">
      <xsl:text>sec_lvl</xsl:text>
      <xsl:value-of select="sectionHeader/@lxir:level"/>
      <xsl:text>_n_</xsl:text>
      <xsl:value-of select="count(preceding-sibling::section)+1"/>
    </xsl:variable>
    
    <a name="{$idSection}"/>
    <div class="{name()}" idSection="{$idSection}">
      <xsl:attribute name="sectionLevel">
         <xsl:value-of select="sectionHeader/@lxir:level"/>
      </xsl:attribute>
      <xsl:apply-templates/>
    </div>
    <!-- line break for better debug -->
    <xsl:text>&#xa;</xsl:text>
  </xsl:template>

  <xsl:template match="sectionHeader">
    <xsl:variable name="classname">
      <xsl:value-of select="name()"/>
    </xsl:variable>
    <div class="{$classname}">
      <xsl:apply-templates select="sectionMark"/>
       <xsl:apply-templates select="sectionTitle"/>
    </div>
    <!-- line break for better debug -->
    <xsl:text>&#xa;</xsl:text>
  </xsl:template>

  <xsl:template match="sectionMark|sectionTitle">
    <span class="{name()}">
      <xsl:apply-templates/>
    </span>
  </xsl:template>
  

<xsl:template match="chapter">
  <div class="{name()}">
    <xsl:apply-templates select="*|@*"/>
  </div>
  <!-- line break for better debug -->
  <xsl:text>&#xa;</xsl:text>
</xsl:template>

</xsl:stylesheet>
