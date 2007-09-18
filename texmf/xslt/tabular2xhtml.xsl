<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" 
		xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
		xmlns="http://www.w3.org/1999/xhtml"
		xmlns:lxir="http://www.latex-lxir.org">
<!-- Stylesheet for transforming table from LXir XML to xhtml table-->

<xsl:template match="tabular|array">
  <!-- Determination du type de frame :
    * void: No sides. This is the default value.
    * above: The top side only.
    * below: The bottom side only.
    * hsides: The top and bottom sides only.
    * vsides: The right and left sides only.
    * lhs: The left-hand side only.
    * rhs: The right-hand side only.
    * box: All four sides.
    * border: All four sides.
    -->
  <xsl:variable name="topline">
    <!-- A  hline preceding the first rowGroup -->
    <xsl:if test="rowGroup[1]/preceding-sibling::hline">
      <xsl:text>1</xsl:text>
    </xsl:if>
  </xsl:variable>

  <xsl:variable name="bottomline">
    <!-- A  hline followinf the last rowGroup -->
    <xsl:if test="rowGroup[last()]/following-sibling::hline">
      <xsl:text>1</xsl:text>
    </xsl:if>
  </xsl:variable>

  <xsl:variable name="leftline">
    <!-- A vline first element of columnsModel -->
    <xsl:if test="name(columnsModel/*[1]) = 'vline'">
      <xsl:text>1</xsl:text>
      <xsl:message>vline left</xsl:message>
    </xsl:if>
  </xsl:variable>

  <xsl:variable name="rightline">
    <!-- A vline last element of columnsModel -->
    <xsl:if test="name(columnsModel/*[last()]) = 'vline'">
      <xsl:text>1</xsl:text>
      <xsl:message>vline right</xsl:message>
    </xsl:if>
  </xsl:variable>

  <xsl:variable name="frame">
    <xsl:choose>
      <xsl:when test="$topline=1 and $bottomline=1 and $leftline=1 and $rightline=1">
	<xsl:text>box</xsl:text>
      </xsl:when>
      <xsl:when test="$topline=1 and $bottomline=1">
	<xsl:text>hsides</xsl:text>
      </xsl:when>
      <xsl:when test="$leftline=1 and $rightline=1">
	<xsl:text>vsides</xsl:text>
      </xsl:when>
     <xsl:when test="$topline=1">
	<xsl:text>above</xsl:text>
      </xsl:when>
      <xsl:when test="$bottomline=1">
	<xsl:text>below</xsl:text>
      </xsl:when>
      <xsl:when test="$leftline=1">
	<xsl:text>lhs</xsl:text>
      </xsl:when>
      <xsl:when test="$rightline=1">
	<xsl:text>rhs</xsl:text>
      </xsl:when>
      <xsl:otherwise>
	<xsl:text>void</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:variable>
  <table class="table" rules="groups" frame="{$frame}">
    <xsl:apply-templates/>
  </table>
</xsl:template>

<xsl:template match="vline"/>
<xsl:template match="hline"/>
<xsl:template match="cline"/>

<xsl:template match="columnsModel">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="columnsModel/columnsGroup">
  <colgroup>
    <xsl:apply-templates/>
  </colgroup>
</xsl:template>

<xsl:template match="columnsModel/columnsGroup/col">
  <col>
    <xsl:copy-of select="@lxir:align"/>
  </col>
</xsl:template>

<xsl:template match="columnsModel/columnsGroup/par">
  <col align="justify"/>
</xsl:template>


<xsl:template match="columnsModel/columnsGroup/intercolumnDef">
  <col/>
</xsl:template>

<xsl:template match="rowGroup">
  <tbody>
    <xsl:apply-templates/>
  </tbody>
</xsl:template>
<xsl:template match="row">
  <tr>
    <xsl:apply-templates/>
  </tr>
</xsl:template>

<xsl:template match="multicolumn">
  <td>
    <xsl:attribute name="colspan">
      <xsl:value-of select="@lxir:span"/>
     </xsl:attribute>
     <xsl:attribute name="align">
       <xsl:choose>
	 <xsl:when test="columnsModel/col/@lxir:align">
	   <xsl:value-of select="columnsModel/col/@lxir:align"/>
	 </xsl:when>
	 <xsl:otherwise>
	   <xsl:text>justify</xsl:text>
	 </xsl:otherwise>
       </xsl:choose>
     </xsl:attribute>
     <xsl:apply-templates select="cell/*|cell/text()"/>
  </td>
</xsl:template>

<xsl:template match="intercolumn">
  <td>
    <xsl:apply-templates/>
  </td>
</xsl:template>

<xsl:template match="cell">
  <td>
    <xsl:apply-templates/>
  </td>
</xsl:template>

</xsl:stylesheet>
