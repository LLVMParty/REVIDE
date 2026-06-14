; ModuleID = 'CFG.bc'
source_filename = "CFG.cpp"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc18.0.40629"

$"??_C@_0BD@IBHCJJML@Usage?3?5test?5number?$AA@" = comdat any

@"??_C@_0BD@IBHCJJML@Usage?3?5test?5number?$AA@" = linkonce_odr dso_local unnamed_addr constant [19 x i8] c"Usage: test number\00", comdat, align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @fn_cfg(i32 %n, i32 %m) #0 !dbg !369 {
entry:
  %m.addr = alloca i32, align 4
  %n.addr = alloca i32, align 4
  store i32 %m, ptr %m.addr, align 4
    #dbg_declare(ptr %m.addr, !372, !DIExpression(), !373)
  store i32 %n, ptr %n.addr, align 4
    #dbg_declare(ptr %n.addr, !374, !DIExpression(), !373)
  %0 = load i32, ptr %n.addr, align 4, !dbg !375
  %and = and i32 %0, 3, !dbg !375
  %cmp = icmp eq i32 %and, 0, !dbg !375
  br i1 %cmp, label %if.then, label %if.else7, !dbg !375

if.then:                                          ; preds = %entry
  %1 = load i32, ptr %n.addr, align 4, !dbg !376
  %add = add nsw i32 %1, 6, !dbg !376
  store i32 %add, ptr %n.addr, align 4, !dbg !376
  %2 = load i32, ptr %n.addr, align 4, !dbg !379
  %3 = load i32, ptr %m.addr, align 4, !dbg !379
  %cmp1 = icmp slt i32 %2, %3, !dbg !379
  br i1 %cmp1, label %if.then2, label %if.else, !dbg !379

if.then2:                                         ; preds = %if.then
  %4 = load i32, ptr %n.addr, align 4, !dbg !380
  %5 = load i32, ptr %n.addr, align 4, !dbg !380
  %add3 = add nsw i32 %4, %5, !dbg !380
  %6 = load i32, ptr %m.addr, align 4, !dbg !380
  %add4 = add nsw i32 %6, %add3, !dbg !380
  store i32 %add4, ptr %m.addr, align 4, !dbg !380
  br label %if.end, !dbg !383

if.else:                                          ; preds = %if.then
  %7 = load i32, ptr %m.addr, align 4, !dbg !384
  %8 = load i32, ptr %m.addr, align 4, !dbg !384
  %add5 = add nsw i32 %7, %8, !dbg !384
  %9 = load i32, ptr %n.addr, align 4, !dbg !384
  %add6 = add nsw i32 %9, %add5, !dbg !384
  store i32 %add6, ptr %n.addr, align 4, !dbg !384
  br label %if.end, !dbg !386

if.end:                                           ; preds = %if.else, %if.then2
  br label %if.end13, !dbg !387

if.else7:                                         ; preds = %entry
  %10 = load i32, ptr %n.addr, align 4, !dbg !388
  %mul = mul nsw i32 %10, 2, !dbg !388
  %11 = load i32, ptr %m.addr, align 4, !dbg !388
  %cmp8 = icmp slt i32 %mul, %11, !dbg !388
  br i1 %cmp8, label %if.then9, label %if.else11, !dbg !388

if.then9:                                         ; preds = %if.else7
  %12 = load i32, ptr %n.addr, align 4, !dbg !389
  %13 = load i32, ptr %m.addr, align 4, !dbg !389
  %add10 = add nsw i32 %13, %12, !dbg !389
  store i32 %add10, ptr %m.addr, align 4, !dbg !389
  br label %if.end12, !dbg !392

if.else11:                                        ; preds = %if.else7
  %14 = load i32, ptr %n.addr, align 4, !dbg !393
  %15 = load i32, ptr %m.addr, align 4, !dbg !393
  %sub = sub nsw i32 %15, %14, !dbg !393
  store i32 %sub, ptr %m.addr, align 4, !dbg !393
  br label %if.end12, !dbg !395

if.end12:                                         ; preds = %if.else11, %if.then9
  br label %if.end13, !dbg !396

if.end13:                                         ; preds = %if.end12, %if.end
  %16 = load i32, ptr %m.addr, align 4, !dbg !397
  %shl = shl i32 %16, 5, !dbg !397
  %17 = load i32, ptr %n.addr, align 4, !dbg !397
  %xor = xor i32 %shl, %17, !dbg !397
  ret i32 %xor, !dbg !397
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @fn_switch(i32 %arg) #0 !dbg !398 {
entry:
  %retval = alloca i32, align 4
  %arg.addr = alloca i32, align 4
  store i32 %arg, ptr %arg.addr, align 4
    #dbg_declare(ptr %arg.addr, !399, !DIExpression(), !400)
  %0 = load i32, ptr %arg.addr, align 4, !dbg !401
  switch i32 %0, label %sw.default [
    i32 1, label %sw.bb
    i32 2, label %sw.bb1
    i32 3, label %sw.bb2
    i32 4, label %sw.bb3
    i32 5, label %sw.bb4
    i32 6, label %sw.bb5
    i32 7, label %sw.bb7
    i32 8, label %sw.bb8
    i32 9, label %sw.bb9
    i32 10, label %sw.bb10
    i32 11, label %sw.bb11
  ], !dbg !401

sw.bb:                                            ; preds = %entry
  %1 = load i32, ptr %arg.addr, align 4, !dbg !402
  %rem = srem i32 %1, 33, !dbg !402
  store i32 %rem, ptr %retval, align 4, !dbg !402
  br label %return, !dbg !402

sw.bb1:                                           ; preds = %entry
  store i32 312, ptr %retval, align 4, !dbg !404
  br label %return, !dbg !404

sw.bb2:                                           ; preds = %entry
  store i32 123, ptr %retval, align 4, !dbg !405
  br label %return, !dbg !405

sw.bb3:                                           ; preds = %entry
  %2 = load i32, ptr %arg.addr, align 4, !dbg !406
  %3 = load i32, ptr %arg.addr, align 4, !dbg !406
  %add = add nsw i32 %2, %3, !dbg !406
  store i32 %add, ptr %retval, align 4, !dbg !406
  br label %return, !dbg !406

sw.bb4:                                           ; preds = %entry
  %4 = load i32, ptr %arg.addr, align 4, !dbg !407
  %mul = mul nsw i32 %4, 42, !dbg !407
  store i32 %mul, ptr %retval, align 4, !dbg !407
  br label %return, !dbg !407

sw.bb5:                                           ; preds = %entry
  %5 = load i32, ptr %arg.addr, align 4, !dbg !408
  %add6 = add nsw i32 %5, 37, !dbg !408
  store i32 %add6, ptr %retval, align 4, !dbg !408
  br label %return, !dbg !408

sw.bb7:                                           ; preds = %entry
  store i32 8471, ptr %retval, align 4, !dbg !409
  br label %return, !dbg !409

sw.bb8:                                           ; preds = %entry
  %6 = load i32, ptr %arg.addr, align 4, !dbg !410
  %xor = xor i32 93847, %6, !dbg !410
  store i32 %xor, ptr %retval, align 4, !dbg !410
  br label %return, !dbg !410

sw.bb9:                                           ; preds = %entry
  %7 = load i32, ptr %arg.addr, align 4, !dbg !411
  %and = and i32 348743, %7, !dbg !411
  store i32 %and, ptr %retval, align 4, !dbg !411
  br label %return, !dbg !411

sw.bb10:                                          ; preds = %entry
  store i32 1354, ptr %retval, align 4, !dbg !412
  br label %return, !dbg !412

sw.bb11:                                          ; preds = %entry
  store i32 2387, ptr %retval, align 4, !dbg !413
  br label %return, !dbg !413

sw.default:                                       ; preds = %entry
  store i32 1337, ptr %retval, align 4, !dbg !414
  br label %return, !dbg !414

return:                                           ; preds = %sw.default, %sw.bb11, %sw.bb10, %sw.bb9, %sw.bb8, %sw.bb7, %sw.bb5, %sw.bb4, %sw.bb3, %sw.bb2, %sw.bb1, %sw.bb
  %8 = load i32, ptr %retval, align 4, !dbg !415
  ret i32 %8, !dbg !415
}

; Function Attrs: noinline norecurse optnone uwtable
define dso_local i32 @main(i32 %argc, ptr %argv) #1 !dbg !416 {
entry:
  %retval = alloca i32, align 4
  %argv.addr = alloca ptr, align 8
  %argc.addr = alloca i32, align 4
  %n = alloca i32, align 4
  %x = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  store ptr %argv, ptr %argv.addr, align 8
    #dbg_declare(ptr %argv.addr, !419, !DIExpression(), !420)
  store i32 %argc, ptr %argc.addr, align 4
    #dbg_declare(ptr %argc.addr, !421, !DIExpression(), !420)
  %0 = load i32, ptr %argc.addr, align 4, !dbg !422
  %cmp = icmp slt i32 %0, 2, !dbg !422
  br i1 %cmp, label %if.then, label %if.end, !dbg !422

if.then:                                          ; preds = %entry
  %call = call i32 @puts(ptr @"??_C@_0BD@IBHCJJML@Usage?3?5test?5number?$AA@"), !dbg !423
  store i32 1, ptr %retval, align 4, !dbg !426
  br label %return, !dbg !426

if.end:                                           ; preds = %entry
    #dbg_declare(ptr %n, !427, !DIExpression(), !428)
  %1 = load ptr, ptr %argv.addr, align 8, !dbg !428
  %arrayidx = getelementptr inbounds ptr, ptr %1, i64 1, !dbg !428
  %2 = load ptr, ptr %arrayidx, align 8, !dbg !428
  %call1 = call i32 @atoi(ptr %2), !dbg !428
  store i32 %call1, ptr %n, align 4, !dbg !428
    #dbg_declare(ptr %x, !429, !DIExpression(), !430)
  %3 = load i32, ptr %n, align 4, !dbg !430
  %call2 = call i32 @fn_switch(i32 %3), !dbg !430
  store i32 %call2, ptr %x, align 4, !dbg !430
  %4 = load i32, ptr %x, align 4, !dbg !431
  %5 = load i32, ptr %n, align 4, !dbg !431
  %call3 = call i32 @fn_cfg(i32 %5, i32 %4), !dbg !431
  store i32 %call3, ptr %retval, align 4, !dbg !431
  br label %return, !dbg !431

return:                                           ; preds = %if.end, %if.then
  %6 = load i32, ptr %retval, align 4, !dbg !432
  ret i32 %6, !dbg !432
}

declare dso_local i32 @puts(ptr) #2

declare dso_local i32 @atoi(ptr) #2

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noinline norecurse optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.dbg.cu = !{!0}
!llvm.linker.options = !{!360, !361, !362, !363}
!llvm.module.flags = !{!364, !365, !366, !367}
!llvm.ident = !{!368}

!0 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_11, file: !1, producer: "clang version 10.0.0 ", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, imports: !3, nameTableKind: None)
!1 = !DIFile(filename: "CFG.cpp", directory: "D:\\CodeBlocks\\llvm10-install-full-v2\\bin", checksumkind: CSK_MD5, checksum: "fb2f9414326ef533582088864c178e8c")
!2 = !{}
!3 = !{!4, !23, !26, !30, !33, !34, !39, !43, !45, !47, !49, !51, !56, !60, !66, !70, !74, !78, !83, !87, !89, !94, !100, !104, !110, !112, !116, !120, !124, !126, !130, !134, !138, !140, !144, !146, !148, !152, !156, !160, !164, !168, !170, !172, !178, !182, !186, !190, !192, !194, !198, !200, !204, !210, !214, !218, !223, !228, !230, !234, !241, !245, !254, !258, !262, !266, !270, !274, !278, !284, !288, !292, !294, !298, !303, !308, !312, !317, !323, !327, !328, !332, !334, !338, !340, !342, !347, !352, !356}
!4 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !6, file: !7, line: 36)
!5 = !DINamespace(name: "std", scope: null)
!6 = !DIDerivedType(tag: DW_TAG_typedef, name: "_iobuf", file: !7, line: 32, baseType: !8)
!7 = !DIFile(filename: "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\INCLUDE\\cstdio", directory: "", checksumkind: CSK_MD5, checksum: "646cc9b877b99ed6682a49083690366d")
!8 = !DIDerivedType(tag: DW_TAG_typedef, name: "FILE", file: !9, line: 73, baseType: !10)
!9 = !DIFile(filename: "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\INCLUDE\\stdio.h", directory: "", checksumkind: CSK_MD5, checksum: "1d9e89fd1986cb456ab46e37741cf29a")
!10 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "_iobuf", file: !9, line: 63, size: 384, flags: DIFlagTypePassByValue, elements: !11, identifier: ".?AU_iobuf@@")
!11 = !{!12, !15, !17, !18, !19, !20, !21, !22}
!12 = !DIDerivedType(tag: DW_TAG_member, name: "_ptr", scope: !10, file: !9, line: 64, baseType: !13, size: 64)
!13 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !14, size: 64)
!14 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!15 = !DIDerivedType(tag: DW_TAG_member, name: "_cnt", scope: !10, file: !9, line: 65, baseType: !16, size: 32, offset: 64)
!16 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!17 = !DIDerivedType(tag: DW_TAG_member, name: "_base", scope: !10, file: !9, line: 66, baseType: !13, size: 64, offset: 128)
!18 = !DIDerivedType(tag: DW_TAG_member, name: "_flag", scope: !10, file: !9, line: 67, baseType: !16, size: 32, offset: 192)
!19 = !DIDerivedType(tag: DW_TAG_member, name: "_file", scope: !10, file: !9, line: 68, baseType: !16, size: 32, offset: 224)
!20 = !DIDerivedType(tag: DW_TAG_member, name: "_charbuf", scope: !10, file: !9, line: 69, baseType: !16, size: 32, offset: 256)
!21 = !DIDerivedType(tag: DW_TAG_member, name: "_bufsiz", scope: !10, file: !9, line: 70, baseType: !16, size: 32, offset: 288)
!22 = !DIDerivedType(tag: DW_TAG_member, name: "_tmpfname", scope: !10, file: !9, line: 71, baseType: !13, size: 64, offset: 320)
!23 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !24, file: !7, line: 36)
!24 = !DIDerivedType(tag: DW_TAG_typedef, name: "_Mbstatet", file: !25, line: 819, baseType: !16)
!25 = !DIFile(filename: "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\INCLUDE\\yvals.h", directory: "", checksumkind: CSK_MD5, checksum: "35f0fe6454aac88dcc452944c62c9dc7")
!26 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !27, file: !7, line: 38)
!27 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !28, line: 494, baseType: !29)
!28 = !DIFile(filename: "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\INCLUDE\\crtdefs.h", directory: "", checksumkind: CSK_MD5, checksum: "095134dfcab636b8b7121b6b00bfbd51")
!29 = !DIBasicType(name: "long long unsigned int", size: 64, encoding: DW_ATE_unsigned)
!30 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !31, file: !7, line: 38)
!31 = !DIDerivedType(tag: DW_TAG_typedef, name: "fpos_t", file: !9, line: 144, baseType: !32)
!32 = !DIBasicType(name: "long long int", size: 64, encoding: DW_ATE_signed)
!33 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !8, file: !7, line: 38)
!34 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !35, file: !7, line: 39)
!35 = !DISubprogram(name: "clearerr", scope: !9, file: !9, line: 180, type: !36, flags: DIFlagPrototyped, spFlags: 0)
!36 = !DISubroutineType(types: !37)
!37 = !{null, !38}
!38 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !8, size: 64)
!39 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !40, file: !7, line: 39)
!40 = !DISubprogram(name: "fclose", scope: !9, file: !9, line: 184, type: !41, flags: DIFlagPrototyped, spFlags: 0)
!41 = !DISubroutineType(types: !42)
!42 = !{!16, !38}
!43 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !44, file: !7, line: 39)
!44 = !DISubprogram(name: "feof", scope: !9, file: !9, line: 189, type: !41, flags: DIFlagPrototyped, spFlags: 0)
!45 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !46, file: !7, line: 40)
!46 = !DISubprogram(name: "ferror", scope: !9, file: !9, line: 190, type: !41, flags: DIFlagPrototyped, spFlags: 0)
!47 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !48, file: !7, line: 40)
!48 = !DISubprogram(name: "fflush", scope: !9, file: !9, line: 191, type: !41, flags: DIFlagPrototyped, spFlags: 0)
!49 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !50, file: !7, line: 40)
!50 = !DISubprogram(name: "fgetc", scope: !9, file: !9, line: 192, type: !41, flags: DIFlagPrototyped, spFlags: 0)
!51 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !52, file: !7, line: 41)
!52 = !DISubprogram(name: "fgetpos", scope: !9, file: !9, line: 194, type: !53, flags: DIFlagPrototyped, spFlags: 0)
!53 = !DISubroutineType(types: !54)
!54 = !{!16, !38, !55}
!55 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !31, size: 64)
!56 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !57, file: !7, line: 41)
!57 = !DISubprogram(name: "fgets", scope: !9, file: !9, line: 195, type: !58, flags: DIFlagPrototyped, spFlags: 0)
!58 = !DISubroutineType(types: !59)
!59 = !{!13, !13, !16, !38}
!60 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !61, file: !7, line: 41)
!61 = !DISubprogram(name: "fopen", scope: !9, file: !9, line: 211, type: !62, flags: DIFlagPrototyped, spFlags: 0)
!62 = !DISubroutineType(types: !63)
!63 = !{!38, !64, !64}
!64 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !65, size: 64)
!65 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !14)
!66 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !67, file: !7, line: 42)
!67 = !DISubprogram(name: "fprintf", scope: !9, file: !9, line: 215, type: !68, flags: DIFlagPrototyped, spFlags: 0)
!68 = !DISubroutineType(types: !69)
!69 = !{!16, !38, !64, null}
!70 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !71, file: !7, line: 42)
!71 = !DISubprogram(name: "fputc", scope: !9, file: !9, line: 219, type: !72, flags: DIFlagPrototyped, spFlags: 0)
!72 = !DISubroutineType(types: !73)
!73 = !{!16, !16, !38}
!74 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !75, file: !7, line: 42)
!75 = !DISubprogram(name: "fputs", scope: !9, file: !9, line: 221, type: !76, flags: DIFlagPrototyped, spFlags: 0)
!76 = !DISubroutineType(types: !77)
!77 = !{!16, !64, !38}
!78 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !79, file: !7, line: 43)
!79 = !DISubprogram(name: "fread", scope: !9, file: !9, line: 222, type: !80, flags: DIFlagPrototyped, spFlags: 0)
!80 = !DISubroutineType(types: !81)
!81 = !{!27, !82, !27, !27, !38}
!82 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!83 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !84, file: !7, line: 43)
!84 = !DISubprogram(name: "freopen", scope: !9, file: !9, line: 226, type: !85, flags: DIFlagPrototyped, spFlags: 0)
!85 = !DISubroutineType(types: !86)
!86 = !{!38, !64, !64, !38}
!87 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !88, file: !7, line: 43)
!88 = !DISubprogram(name: "fscanf", scope: !9, file: !9, line: 230, type: !68, flags: DIFlagPrototyped, spFlags: 0)
!89 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !90, file: !7, line: 44)
!90 = !DISubprogram(name: "fseek", scope: !9, file: !9, line: 240, type: !91, flags: DIFlagPrototyped, spFlags: 0)
!91 = !DISubroutineType(types: !92)
!92 = !{!16, !38, !93, !16}
!93 = !DIBasicType(name: "long int", size: 32, encoding: DW_ATE_signed)
!94 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !95, file: !7, line: 44)
!95 = !DISubprogram(name: "fsetpos", scope: !9, file: !9, line: 239, type: !96, flags: DIFlagPrototyped, spFlags: 0)
!96 = !DISubroutineType(types: !97)
!97 = !{!16, !38, !98}
!98 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !99, size: 64)
!99 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !31)
!100 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !101, file: !7, line: 44)
!101 = !DISubprogram(name: "ftell", scope: !9, file: !9, line: 241, type: !102, flags: DIFlagPrototyped, spFlags: 0)
!102 = !DISubroutineType(types: !103)
!103 = !{!93, !38}
!104 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !105, file: !7, line: 45)
!105 = !DISubprogram(name: "fwrite", scope: !9, file: !9, line: 246, type: !106, flags: DIFlagPrototyped, spFlags: 0)
!106 = !DISubroutineType(types: !107)
!107 = !{!27, !108, !27, !27, !38}
!108 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !109, size: 64)
!109 = !DIDerivedType(tag: DW_TAG_const_type, baseType: null)
!110 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !111, file: !7, line: 45)
!111 = !DISubprogram(name: "getc", scope: !9, file: !9, line: 247, type: !41, flags: DIFlagPrototyped, spFlags: 0)
!112 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !113, file: !7, line: 45)
!113 = !DISubprogram(name: "getchar", scope: !9, file: !9, line: 248, type: !114, flags: DIFlagPrototyped, spFlags: 0)
!114 = !DISubroutineType(types: !115)
!115 = !{!16}
!116 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !117, file: !7, line: 46)
!117 = !DISubprogram(name: "gets", scope: !9, file: !9, line: 254, type: !118, flags: DIFlagPrototyped, spFlags: 0)
!118 = !DISubroutineType(types: !119)
!119 = !{!13, !13}
!120 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !121, file: !7, line: 46)
!121 = !DISubprogram(name: "perror", scope: !9, file: !9, line: 258, type: !122, flags: DIFlagPrototyped, spFlags: 0)
!122 = !DISubroutineType(types: !123)
!123 = !{null, !64}
!124 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !125, file: !7, line: 47)
!125 = !DISubprogram(name: "putc", scope: !9, file: !9, line: 268, type: !72, flags: DIFlagPrototyped, spFlags: 0)
!126 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !127, file: !7, line: 47)
!127 = !DISubprogram(name: "putchar", scope: !9, file: !9, line: 269, type: !128, flags: DIFlagPrototyped, spFlags: 0)
!128 = !DISubroutineType(types: !129)
!129 = !{!16, !16}
!130 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !131, file: !7, line: 48)
!131 = !DISubprogram(name: "printf", scope: !9, file: !9, line: 264, type: !132, flags: DIFlagPrototyped, spFlags: 0)
!132 = !DISubroutineType(types: !133)
!133 = !{!16, !64, null}
!134 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !135, file: !7, line: 48)
!135 = !DISubprogram(name: "puts", scope: !9, file: !9, line: 270, type: !136, flags: DIFlagPrototyped, spFlags: 0)
!136 = !DISubroutineType(types: !137)
!137 = !{!16, !64}
!138 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !139, file: !7, line: 48)
!139 = !DISubprogram(name: "remove", scope: !9, file: !9, line: 274, type: !136, flags: DIFlagPrototyped, spFlags: 0)
!140 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !141, file: !7, line: 49)
!141 = !DISubprogram(name: "rename", scope: !9, file: !9, line: 275, type: !142, flags: DIFlagPrototyped, spFlags: 0)
!142 = !DISubroutineType(types: !143)
!143 = !{!16, !64, !64}
!144 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !145, file: !7, line: 49)
!145 = !DISubprogram(name: "rewind", scope: !9, file: !9, line: 281, type: !36, flags: DIFlagPrototyped, spFlags: 0)
!146 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !147, file: !7, line: 49)
!147 = !DISubprogram(name: "scanf", scope: !9, file: !9, line: 283, type: !132, flags: DIFlagPrototyped, spFlags: 0)
!148 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !149, file: !7, line: 50)
!149 = !DISubprogram(name: "setbuf", scope: !9, file: !9, line: 292, type: !150, flags: DIFlagPrototyped, spFlags: 0)
!150 = !DISubroutineType(types: !151)
!151 = !{null, !38, !13}
!152 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !153, file: !7, line: 50)
!153 = !DISubprogram(name: "setvbuf", scope: !9, file: !9, line: 296, type: !154, flags: DIFlagPrototyped, spFlags: 0)
!154 = !DISubroutineType(types: !155)
!155 = !{!16, !38, !13, !16, !27}
!156 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !157, file: !7, line: 50)
!157 = !DISubprogram(name: "sprintf", scope: !9, file: !9, line: 356, type: !158, flags: DIFlagPrototyped, spFlags: 0)
!158 = !DISubroutineType(types: !159)
!159 = !{!16, !13, !64, null}
!160 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !161, file: !7, line: 51)
!161 = !DISubprogram(name: "sscanf", scope: !9, file: !9, line: 304, type: !162, flags: DIFlagPrototyped, spFlags: 0)
!162 = !DISubroutineType(types: !163)
!163 = !{!16, !64, !64, null}
!164 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !165, file: !7, line: 51)
!165 = !DISubprogram(name: "tmpfile", scope: !9, file: !9, line: 317, type: !166, flags: DIFlagPrototyped, spFlags: 0)
!166 = !DISubroutineType(types: !167)
!167 = !{!38}
!168 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !169, file: !7, line: 51)
!169 = !DISubprogram(name: "tmpnam", scope: !9, file: !9, line: 323, type: !118, flags: DIFlagPrototyped, spFlags: 0)
!170 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !171, file: !7, line: 52)
!171 = !DISubprogram(name: "ungetc", scope: !9, file: !9, line: 324, type: !72, flags: DIFlagPrototyped, spFlags: 0)
!172 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !173, file: !7, line: 52)
!173 = !DISubprogram(name: "vfprintf", scope: !9, file: !9, line: 325, type: !174, flags: DIFlagPrototyped, spFlags: 0)
!174 = !DISubroutineType(types: !175)
!175 = !{!16, !38, !64, !176}
!176 = !DIDerivedType(tag: DW_TAG_typedef, name: "va_list", file: !177, line: 59, baseType: !13)
!177 = !DIFile(filename: "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\INCLUDE\\vadefs.h", directory: "", checksumkind: CSK_MD5, checksum: "915f3bc56e10589b0d5dba9a3e12d945")
!178 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !179, file: !7, line: 52)
!179 = !DISubprogram(name: "vprintf", scope: !9, file: !9, line: 331, type: !180, flags: DIFlagPrototyped, spFlags: 0)
!180 = !DISubroutineType(types: !181)
!181 = !{!16, !64, !176}
!182 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !183, file: !7, line: 53)
!183 = !DISubprogram(name: "vsprintf", scope: !9, file: !9, line: 356, type: !184, flags: DIFlagPrototyped, spFlags: 0)
!184 = !DISubroutineType(types: !185)
!185 = !{!16, !13, !64, !176}
!186 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !187, file: !7, line: 55)
!187 = !DISubprogram(name: "vsnprintf", scope: !9, file: !9, line: 337, type: !188, flags: DIFlagPrototyped, spFlags: 0)
!188 = !DISubroutineType(types: !189)
!189 = !{!16, !13, !27, !64, !176}
!190 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !191, file: !7, line: 56)
!191 = !DISubprogram(name: "vfscanf", scope: !9, file: !9, line: 326, type: !174, flags: DIFlagPrototyped, spFlags: 0)
!192 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !193, file: !7, line: 56)
!193 = !DISubprogram(name: "vscanf", scope: !9, file: !9, line: 332, type: !180, flags: DIFlagPrototyped, spFlags: 0)
!194 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !195, file: !7, line: 56)
!195 = !DISubprogram(name: "vsscanf", scope: !9, file: !9, line: 357, type: !196, flags: DIFlagPrototyped, spFlags: 0)
!196 = !DISubroutineType(types: !197)
!197 = !{!16, !64, !64, !176}
!198 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !27, file: !199, line: 18)
!199 = !DIFile(filename: "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\INCLUDE\\cstdlib", directory: "", checksumkind: CSK_MD5, checksum: "9e027c03aaa48df8d0be709048842d4c")
!200 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !201, file: !199, line: 18)
!201 = !DIDerivedType(tag: DW_TAG_typedef, name: "div_t", file: !202, line: 78, baseType: !203)
!202 = !DIFile(filename: "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\INCLUDE\\stdlib.h", directory: "", checksumkind: CSK_MD5, checksum: "285560e757f336683936884a88718aec")
!203 = !DICompositeType(tag: DW_TAG_structure_type, name: "_div_t", file: !202, line: 75, flags: DIFlagFwdDecl, identifier: ".?AU_div_t@@")
!204 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !205, file: !199, line: 18)
!205 = !DIDerivedType(tag: DW_TAG_typedef, name: "ldiv_t", file: !202, line: 83, baseType: !206)
!206 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "_ldiv_t", file: !202, line: 80, size: 64, flags: DIFlagTypePassByValue, elements: !207, identifier: ".?AU_ldiv_t@@")
!207 = !{!208, !209}
!208 = !DIDerivedType(tag: DW_TAG_member, name: "quot", scope: !206, file: !202, line: 81, baseType: !93, size: 32)
!209 = !DIDerivedType(tag: DW_TAG_member, name: "rem", scope: !206, file: !202, line: 82, baseType: !93, size: 32, offset: 32)
!210 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !211, file: !199, line: 19)
!211 = !DISubprogram(name: "abort", scope: !202, file: !202, line: 379, type: !212, flags: DIFlagPrototyped | DIFlagNoReturn, spFlags: 0)
!212 = !DISubroutineType(types: !213)
!213 = !{null}
!214 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !215, file: !199, line: 19)
!215 = !DISubprogram(name: "abs", linkageName: "?abs@@YA_J_J@Z", scope: !202, file: !202, line: 501, type: !216, flags: DIFlagPrototyped, spFlags: 0)
!216 = !DISubroutineType(types: !217)
!217 = !{!32, !32}
!218 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !219, file: !199, line: 19)
!219 = !DISubprogram(name: "atexit", scope: !202, file: !202, line: 426, type: !220, flags: DIFlagPrototyped, spFlags: 0)
!220 = !DISubroutineType(types: !221)
!221 = !{!16, !222}
!222 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !212, size: 64)
!223 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !224, file: !199, line: 20)
!224 = !DISubprogram(name: "atof", scope: !202, file: !202, line: 428, type: !225, flags: DIFlagPrototyped, spFlags: 0)
!225 = !DISubroutineType(types: !226)
!226 = !{!227, !64}
!227 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!228 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !229, file: !199, line: 20)
!229 = !DISubprogram(name: "atoi", scope: !202, file: !202, line: 430, type: !136, flags: DIFlagPrototyped, spFlags: 0)
!230 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !231, file: !199, line: 20)
!231 = !DISubprogram(name: "atol", scope: !202, file: !202, line: 432, type: !232, flags: DIFlagPrototyped, spFlags: 0)
!232 = !DISubroutineType(types: !233)
!233 = !{!93, !64}
!234 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !235, file: !199, line: 21)
!235 = !DISubprogram(name: "bsearch", scope: !202, file: !202, line: 443, type: !236, flags: DIFlagPrototyped, spFlags: 0)
!236 = !DISubroutineType(types: !237)
!237 = !{!82, !108, !108, !27, !27, !238}
!238 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !239, size: 64)
!239 = !DISubroutineType(types: !240)
!240 = !{!16, !108, !108}
!241 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !242, file: !199, line: 21)
!242 = !DISubprogram(name: "calloc", scope: !202, file: !202, line: 640, type: !243, flags: DIFlagPrototyped, spFlags: 0)
!243 = !DISubroutineType(types: !244)
!244 = !{!82, !27, !27}
!245 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !246, file: !199, line: 21)
!246 = !DISubprogram(name: "div", linkageName: "?div@@YA?AU_lldiv_t@@_J0@Z", scope: !202, file: !202, line: 509, type: !247, flags: DIFlagPrototyped, spFlags: 0)
!247 = !DISubroutineType(types: !248)
!248 = !{!249, !32, !32}
!249 = !DIDerivedType(tag: DW_TAG_typedef, name: "lldiv_t", file: !202, line: 88, baseType: !250)
!250 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "_lldiv_t", file: !202, line: 85, size: 128, flags: DIFlagTypePassByValue, elements: !251, identifier: ".?AU_lldiv_t@@")
!251 = !{!252, !253}
!252 = !DIDerivedType(tag: DW_TAG_member, name: "quot", scope: !250, file: !202, line: 86, baseType: !32, size: 64)
!253 = !DIDerivedType(tag: DW_TAG_member, name: "rem", scope: !250, file: !202, line: 87, baseType: !32, size: 64, offset: 64)
!254 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !255, file: !199, line: 22)
!255 = !DISubprogram(name: "exit", scope: !202, file: !202, line: 376, type: !256, flags: DIFlagPrototyped | DIFlagNoReturn, spFlags: 0)
!256 = !DISubroutineType(types: !257)
!257 = !{null, !16}
!258 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !259, file: !199, line: 22)
!259 = !DISubprogram(name: "free", scope: !202, file: !202, line: 641, type: !260, flags: DIFlagPrototyped, spFlags: 0)
!260 = !DISubroutineType(types: !261)
!261 = !{null, !82}
!262 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !263, file: !199, line: 23)
!263 = !DISubprogram(name: "labs", scope: !202, file: !202, line: 385, type: !264, flags: DIFlagPrototyped, spFlags: 0)
!264 = !DISubroutineType(types: !265)
!265 = !{!93, !93}
!266 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !267, file: !199, line: 23)
!267 = !DISubprogram(name: "ldiv", scope: !202, file: !202, line: 492, type: !268, flags: DIFlagPrototyped, spFlags: 0)
!268 = !DISubroutineType(types: !269)
!269 = !{!205, !93, !93}
!270 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !271, file: !199, line: 23)
!271 = !DISubprogram(name: "malloc", scope: !202, file: !202, line: 642, type: !272, flags: DIFlagPrototyped, spFlags: 0)
!272 = !DISubroutineType(types: !273)
!273 = !{!82, !27}
!274 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !275, file: !199, line: 24)
!275 = !DISubprogram(name: "mblen", scope: !202, file: !202, line: 518, type: !276, flags: DIFlagPrototyped, spFlags: 0)
!276 = !DISubroutineType(types: !277)
!277 = !{!16, !64, !27}
!278 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !279, file: !199, line: 24)
!279 = !DISubprogram(name: "mbstowcs", scope: !202, file: !202, line: 528, type: !280, flags: DIFlagPrototyped, spFlags: 0)
!280 = !DISubroutineType(types: !281)
!281 = !{!27, !282, !64, !27}
!282 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !283, size: 64)
!283 = !DIBasicType(name: "wchar_t", size: 16, encoding: DW_ATE_unsigned)
!284 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !285, file: !199, line: 24)
!285 = !DISubprogram(name: "mbtowc", scope: !202, file: !202, line: 524, type: !286, flags: DIFlagPrototyped, spFlags: 0)
!286 = !DISubroutineType(types: !287)
!287 = !{!16, !282, !64, !27}
!288 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !289, file: !199, line: 25)
!289 = !DISubprogram(name: "qsort", scope: !202, file: !202, line: 452, type: !290, flags: DIFlagPrototyped, spFlags: 0)
!290 = !DISubroutineType(types: !291)
!291 = !{null, !82, !27, !27, !238}
!292 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !293, file: !199, line: 25)
!293 = !DISubprogram(name: "rand", scope: !202, file: !202, line: 534, type: !114, flags: DIFlagPrototyped, spFlags: 0)
!294 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !295, file: !199, line: 25)
!295 = !DISubprogram(name: "realloc", scope: !202, file: !202, line: 644, type: !296, flags: DIFlagPrototyped, spFlags: 0)
!296 = !DISubroutineType(types: !297)
!297 = !{!82, !82, !27}
!298 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !299, file: !199, line: 26)
!299 = !DISubprogram(name: "srand", scope: !202, file: !202, line: 541, type: !300, flags: DIFlagPrototyped, spFlags: 0)
!300 = !DISubroutineType(types: !301)
!301 = !{null, !302}
!302 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!303 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !304, file: !199, line: 26)
!304 = !DISubprogram(name: "strtod", scope: !202, file: !202, line: 542, type: !305, flags: DIFlagPrototyped, spFlags: 0)
!305 = !DISubroutineType(types: !306)
!306 = !{!227, !64, !307}
!307 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !13, size: 64)
!308 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !309, file: !199, line: 26)
!309 = !DISubprogram(name: "strtol", scope: !202, file: !202, line: 544, type: !310, flags: DIFlagPrototyped, spFlags: 0)
!310 = !DISubroutineType(types: !311)
!311 = !{!93, !64, !307, !16}
!312 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !313, file: !199, line: 27)
!313 = !DISubprogram(name: "strtoul", scope: !202, file: !202, line: 548, type: !314, flags: DIFlagPrototyped, spFlags: 0)
!314 = !DISubroutineType(types: !315)
!315 = !{!316, !64, !307, !16}
!316 = !DIBasicType(name: "long unsigned int", size: 32, encoding: DW_ATE_unsigned)
!317 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !318, file: !199, line: 28)
!318 = !DISubprogram(name: "wcstombs", scope: !202, file: !202, line: 575, type: !319, flags: DIFlagPrototyped, spFlags: 0)
!319 = !DISubroutineType(types: !320)
!320 = !{!27, !13, !321, !27}
!321 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !322, size: 64)
!322 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !283)
!323 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !324, file: !199, line: 28)
!324 = !DISubprogram(name: "wctomb", scope: !202, file: !202, line: 567, type: !325, flags: DIFlagPrototyped, spFlags: 0)
!325 = !DISubroutineType(types: !326)
!326 = !{!16, !13, !283}
!327 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !249, file: !199, line: 30)
!328 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !329, file: !199, line: 33)
!329 = !DISubprogram(name: "getenv", scope: !202, file: !202, line: 462, type: !330, flags: DIFlagPrototyped, spFlags: 0)
!330 = !DISubroutineType(types: !331)
!331 = !{!13, !64}
!332 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !333, file: !199, line: 34)
!333 = !DISubprogram(name: "system", scope: !202, file: !202, line: 560, type: !136, flags: DIFlagPrototyped, spFlags: 0)
!334 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !335, file: !199, line: 37)
!335 = !DISubprogram(name: "atoll", scope: !202, file: !202, line: 434, type: !336, flags: DIFlagPrototyped, spFlags: 0)
!336 = !DISubroutineType(types: !337)
!337 = !{!32, !64}
!338 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !339, file: !199, line: 37)
!339 = !DISubprogram(name: "llabs", scope: !202, file: !202, line: 386, type: !216, flags: DIFlagPrototyped, spFlags: 0)
!340 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !341, file: !199, line: 37)
!341 = !DISubprogram(name: "lldiv", scope: !202, file: !202, line: 493, type: !247, flags: DIFlagPrototyped, spFlags: 0)
!342 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !343, file: !199, line: 38)
!343 = !DISubprogram(name: "strtof", scope: !202, file: !202, line: 554, type: !344, flags: DIFlagPrototyped, spFlags: 0)
!344 = !DISubroutineType(types: !345)
!345 = !{!346, !64, !307}
!346 = !DIBasicType(name: "float", size: 32, encoding: DW_ATE_float)
!347 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !348, file: !199, line: 38)
!348 = !DISubprogram(name: "strtold", scope: !202, file: !202, line: 552, type: !349, flags: DIFlagPrototyped, spFlags: 0)
!349 = !DISubroutineType(types: !350)
!350 = !{!351, !64, !307}
!351 = !DIBasicType(name: "long double", size: 64, encoding: DW_ATE_float)
!352 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !353, file: !199, line: 39)
!353 = !DISubprogram(name: "strtoll", scope: !202, file: !202, line: 546, type: !354, flags: DIFlagPrototyped, spFlags: 0)
!354 = !DISubroutineType(types: !355)
!355 = !{!32, !64, !307, !16}
!356 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !5, entity: !357, file: !199, line: 39)
!357 = !DISubprogram(name: "strtoull", scope: !202, file: !202, line: 550, type: !358, flags: DIFlagPrototyped, spFlags: 0)
!358 = !DISubroutineType(types: !359)
!359 = !{!29, !64, !307, !16}
!360 = !{!"/FAILIFMISMATCH:\22_MSC_VER=1800\22"}
!361 = !{!"/FAILIFMISMATCH:\22_ITERATOR_DEBUG_LEVEL=0\22"}
!362 = !{!"/FAILIFMISMATCH:\22RuntimeLibrary=MT_StaticRelease\22"}
!363 = !{!"/DEFAULTLIB:libcpmt.lib"}
!364 = !{i32 2, !"CodeView", i32 1}
!365 = !{i32 2, !"Debug Info Version", i32 3}
!366 = !{i32 1, !"wchar_size", i32 2}
!367 = !{i32 8, !"PIC Level", i32 2}
!368 = !{!"clang version 10.0.0 "}
!369 = distinct !DISubprogram(name: "fn_cfg", scope: !1, file: !1, line: 6, type: !370, scopeLine: 6, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!370 = !DISubroutineType(types: !371)
!371 = !{!16, !16, !16}
!372 = !DILocalVariable(name: "m", arg: 2, scope: !369, file: !1, line: 6, type: !16)
!373 = !DILocation(line: 6, scope: !369)
!374 = !DILocalVariable(name: "n", arg: 1, scope: !369, file: !1, line: 6, type: !16)
!375 = !DILocation(line: 7, scope: !369)
!376 = !DILocation(line: 8, scope: !377)
!377 = distinct !DILexicalBlock(scope: !378, file: !1, line: 7)
!378 = distinct !DILexicalBlock(scope: !369, file: !1, line: 7)
!379 = !DILocation(line: 9, scope: !377)
!380 = !DILocation(line: 10, scope: !381)
!381 = distinct !DILexicalBlock(scope: !382, file: !1, line: 9)
!382 = distinct !DILexicalBlock(scope: !377, file: !1, line: 9)
!383 = !DILocation(line: 11, scope: !381)
!384 = !DILocation(line: 12, scope: !385)
!385 = distinct !DILexicalBlock(scope: !382, file: !1, line: 11)
!386 = !DILocation(line: 13, scope: !385)
!387 = !DILocation(line: 14, scope: !377)
!388 = !DILocation(line: 15, scope: !378)
!389 = !DILocation(line: 16, scope: !390)
!390 = distinct !DILexicalBlock(scope: !391, file: !1, line: 15)
!391 = distinct !DILexicalBlock(scope: !378, file: !1, line: 15)
!392 = !DILocation(line: 17, scope: !390)
!393 = !DILocation(line: 18, scope: !394)
!394 = distinct !DILexicalBlock(scope: !391, file: !1, line: 17)
!395 = !DILocation(line: 19, scope: !394)
!396 = !DILocation(line: 15, scope: !391)
!397 = !DILocation(line: 20, scope: !369)
!398 = distinct !DISubprogram(name: "fn_switch", scope: !1, file: !1, line: 23, type: !128, scopeLine: 23, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!399 = !DILocalVariable(name: "arg", arg: 1, scope: !398, file: !1, line: 23, type: !16)
!400 = !DILocation(line: 23, scope: !398)
!401 = !DILocation(line: 24, scope: !398)
!402 = !DILocation(line: 26, scope: !403)
!403 = distinct !DILexicalBlock(scope: !398, file: !1, line: 24)
!404 = !DILocation(line: 28, scope: !403)
!405 = !DILocation(line: 30, scope: !403)
!406 = !DILocation(line: 32, scope: !403)
!407 = !DILocation(line: 34, scope: !403)
!408 = !DILocation(line: 36, scope: !403)
!409 = !DILocation(line: 38, scope: !403)
!410 = !DILocation(line: 40, scope: !403)
!411 = !DILocation(line: 42, scope: !403)
!412 = !DILocation(line: 44, scope: !403)
!413 = !DILocation(line: 46, scope: !403)
!414 = !DILocation(line: 48, scope: !403)
!415 = !DILocation(line: 50, scope: !398)
!416 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 52, type: !417, scopeLine: 52, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!417 = !DISubroutineType(types: !418)
!418 = !{!16, !16, !307}
!419 = !DILocalVariable(name: "argv", arg: 2, scope: !416, file: !1, line: 52, type: !307)
!420 = !DILocation(line: 52, scope: !416)
!421 = !DILocalVariable(name: "argc", arg: 1, scope: !416, file: !1, line: 52, type: !16)
!422 = !DILocation(line: 53, scope: !416)
!423 = !DILocation(line: 54, scope: !424)
!424 = distinct !DILexicalBlock(scope: !425, file: !1, line: 53)
!425 = distinct !DILexicalBlock(scope: !416, file: !1, line: 53)
!426 = !DILocation(line: 55, scope: !424)
!427 = !DILocalVariable(name: "n", scope: !416, file: !1, line: 58, type: !16)
!428 = !DILocation(line: 58, scope: !416)
!429 = !DILocalVariable(name: "x", scope: !416, file: !1, line: 59, type: !16)
!430 = !DILocation(line: 59, scope: !416)
!431 = !DILocation(line: 60, scope: !416)
!432 = !DILocation(line: 63, scope: !416)
