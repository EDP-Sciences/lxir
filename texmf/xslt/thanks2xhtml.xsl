<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
		xmlns="http://www.w3.org/1999/xhtml" 
		xmlns:lxir="http://www.latex-lxir.org"
  >
  <!--
      thanks, thanksMark, thanksText, thanksMark/superscript
  -->

  <xsl:template match="thanks">
    <div class="{name()}">
      <xsl:apply-templates/>
    </div>
  </xsl:template>


  <xsl:template match="thanks/thanksMark">
    <span class="{name()}">
      <xsl:value-of select="@lxir:number"/>
    </span>
  </xsl:template>

  <xsl:template match="thanks/thanksText">
    <span class="{name()}">
      <xsl:apply-templates/>
    </span>
  </xsl:template>

  
</xsl:stylesheet>