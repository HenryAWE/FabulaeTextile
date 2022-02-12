# 库设计 / Library Design
## 数据结构 / Data Structure
- 笔画 / Stroke  
  储存一个基本笔画，可以通过锚点连接其他的笔画。锚点可以不在笔画上  
  Stores a basic stroke that can be connected to other strokes via anchors. Anchors can be off the stroke
- 部首 / Radical (Indexing Component)  
  由几个笔画构成的组件  
  Component consisting of several strokes
- 字 / Character  
  包含一个或多个部首或其他汉字作为子部件，并用结构描述符记录子部件的位置  
  Contains one or more radicals or other Chinese characters as subcomponents, and uses the structural descriptors to indicate the position of the subcomponents
